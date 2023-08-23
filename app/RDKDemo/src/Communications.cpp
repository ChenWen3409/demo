/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:23:50
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-23 17:01:14
 * @ Description:
 */
#include "Communications.hpp"

namespace kostal {

Status Communications::init(flexiv::Robot* robotPtr)
{

    Status result;

    // 1st, check robot connection and set robot to plan execution mode, if
    // failed, continue to init socket server and send error status to client
    result = m_robotHandler.buildRobotConnection(robotPtr);
    if (result != SUCCESS) {
        // If failed at the first place, need to report and exit
        k_log->error("The flexiv system failed to initialize the robot!");
        k_log->error("Please recover the robot and then reboot it");
        k_log->error("===================================================");
        m_flexivStatus = FAULT;
        m_seriousErrorFlag = true;
    } else {
        k_log->info("The robot connection is built successfully");
    }
    std::cout << "Conect with testman..." << std::endl;

    // 2nd, try to initialize the socket server and get token from
    // testman's shakehand message
    result = m_service.init();
    if (result != SUCCESS) {
        k_log->error(
            "The flexiv system fails to initialize the socket server, "
            "please check your network");
        // if socket failed, directly return error code and break because
        // there is no need to continue
        m_flexivStatus = FAULT;
        m_seriousErrorFlag = true;
        return result;
    } else {
        k_log->info("The socket connection is built successfully");
    }

    // 3rd, if flexivStatus has been changed from INIT, but the socket server
    // is successfully initialized, return SYSTEM to indicate a hardware fault.
    if (m_flexivStatus != INIT) {
        k_log->error(
            "The flexiv system failed in initialization, please check "
            "ROBOT");
        m_flexivStatus = FAULT;
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);

        return SYSTEM;
    }

    // 4th, if flexivStatus still keeps at INIT, return success and switch
    // flexivStatus to IDLE
    k_log->info("The flexiv system is initialized successfully");
    m_flexivStatus = IDLE;

    return SUCCESS;
}

Status Communications::executeCheck()
{
    Status result;

    // get received message first
    std::string recvMsg = m_service.getRecvMsg();

    // parse the json message to retrieve the value of queryStatus and
    // terminateStatus
    result = m_parserHandler.parseHBJSON(&recvMsg, &m_queryStatus, &m_terminateStatus);
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        k_log->error("The task message is failed to be parsed");

        return result;
    }

    // Check the terminateStatus and turn on the terminate switch if the answer
    // is True or Yes
    std::transform(
        m_terminateStatus.begin(), m_terminateStatus.end(), m_terminateStatus.begin(), ::toupper);
    if (m_terminateStatus == "TRUE" || m_terminateStatus == "YES") {
        m_terminateSwitch = true;
    } else {
        m_terminateSwitch = false;
    }

    // Check the queryStatus and turn on the check status flag if the answer is
    // True or Yes
    std::transform(m_queryStatus.begin(), m_queryStatus.end(), m_queryStatus.begin(), ::toupper);
    if (m_queryStatus == "TRUE" || m_queryStatus == "YES") {
        m_checkStatusFlag = true;
    } else {
        m_checkStatusFlag = false;
    }

    return SUCCESS;
}

Status Communications::executeTask(flexiv::Robot* robotPtr)
{
    Status result;

    // 1st, get received message first
    std::string recvMsg = m_service.getRecvMsg();

    // 2nd, parse the json message to retrieve the value of queryStatus,
    // taskType and taskName
    result = m_parserHandler.parseTaskJSON(&recvMsg, &m_queryStatus, &m_taskType, &m_taskName);
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        k_log->error("The task message is failed to be parsed");
        k_log->error("===================================================");

        return result;
    }

    // 3rd, before executing the task, we need to clear robot data list manually
    // check whether robot has this plan name first and excute
    m_robotDataList.clear();

    std::string planName = m_taskName + "-" + m_taskType + "-MainPlan";
    if (m_robotHandler.checkRobotPlan(robotPtr, planName) == false) {
        return ROBOT;
    }
    // turn on the switch to collect data
    g_collectSwitch = true;

    // excute robot plan
    m_robotHandler.excuteRobotPlan(robotPtr, planName);

    // 4th, run a sync task to execution the workplan and collect the robot data
    // at the same time
    auto robotWorker = std::bind(&kostal::RobotOperations::collectSyncData, m_robotHandler,
        robotPtr, &m_robotData, &m_robotDataList);
    std::thread robotThread(robotWorker);
    // Wait until the program is finished
    while (robotPtr->isBusy()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // turn off the switch to collect data
    g_collectSwitch = false;

    // Wait for threads till the end
    robotThread.join();
    k_log->info("The sync task is finished by scheduler");

    // Excute primitive that let robot back to home pose
    std::cout << "robot back to home" << std::endl;
    m_robotHandler.executeRobotPrimitiveHome(robotPtr);

    // get globar var list
    std::vector<std::string> globalVarList;
    m_robotHandler.getRobotGlobalVar(robotPtr, globalVarList);
    std::cout << "before set global var lsit" << std::endl;
    for (int i = 0; i < globalVarList.size(); i++) {
        std::cout << globalVarList[i] << std::endl;
    }

    // set globar var
    std::string globalVarset = "a = 1";
    m_robotHandler.setRobotGlobalVar(robotPtr, globalVarset);

    // get globar var list
    m_robotHandler.getRobotGlobalVar(robotPtr, globalVarList);
    std::cout << "after set global var lsit" << std::endl;
    for (int i = 0; i < globalVarList.size(); i++) {
        std::cout << globalVarList[i] << std::endl;
    }

    // 5th, write excel files with collected robot data list and spi data list
    result = m_weHandler.writeDataToExcel(m_taskType, m_taskName, &m_robotDataList);
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        k_log->error("The excel file is failed to be generated");
        k_log->error("===================================================");
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);

        return result;
    }

    // 6th, after all these steps, mention that the task execution is finished
    k_log->info("****************************************************");
    k_log->info("The task is executed successfully");
    k_log->info("****************************************************");

    // 7th, after the reinit of the spi device, switch flexivStatus to IDLE
    // and isBiasMode to false again
    m_flexivStatus = IDLE;

    return SUCCESS;
}

void Communications::stateMachine(flexiv::Robot* robotPtr)
{
    Status result;

    // put all the status transition into a closed-loop Finite State Machine
    while (true) {
        // the transition condition is flexivStatus
        switch (m_flexivStatus) {
            case IDLE: {
                // first, we check whether robot has tiny fault, if yes, clear
                // it. If it fails, jump out of the loop
                result = m_robotHandler.clearTinyFault(robotPtr);
                if (result != SUCCESS) {
                    m_flexivStatus = FAULT;
                    m_seriousErrorFlag = true;
                    k_log->error("Please recover the robot and then reboot it");
                    return;
                }

                // manually set these two indicators to default value.
                m_checkStatusFlag = true;
                m_terminateSwitch = false;

                // when m_checkStatusFlag is equal to true, it will stay in this loop
                while (m_checkStatusFlag == true) {

                    // This will be print all over the heartbeat
                    std::cout << "The flexiv system is in idle mode, listening" << std::endl;

                    // set reply msg to "IDLE" and keep answering
                    m_service.setReplyMsg("IDLE");
                    result = m_service.monitor();
                    if (result != SUCCESS) {
                        m_flexivStatus = FAULT;
                        k_log->error(
                            "The flexiv system is having an error in "
                            "heartbeat");
                        k_log->error(
                            "=============================================="
                            "=====");
                        return;
                    }

                    // check the param and update the m_checkStatusFlag
                    result = executeCheck();
                    if (result != SUCCESS) {
                        m_flexivStatus = FAULT;
                        k_log->error(
                            "The flexiv system is having an error in "
                            "querying server status");
                        return;
                    }
                }

                // when m_checkStatusFlag is not equal to true, which means
                // it is time to execute task

                // Check whether client sends terminate signal
                // if the client does not require for terminate
                if (m_terminateSwitch == false) {
                    // set status to BUSY and put the task into a thread pool
                    k_log->info("The flexiv system received a task!");
                    m_flexivStatus = BUSY;
                    boost::asio::post(
                        t_pool, boost::bind(&Communications::executeTask, this, robotPtr));
                    // directly go to BUSY in switch-case
                    break;
                } else {
                    // client is sending a terminate signal, so go to
                    // switch-case DONE directly
                    std::cout << "The flexiv system received a terminate signal!" << std::endl;
                    m_flexivStatus = DONE;
                    break;
                }
            }

            case BUSY: {
                // set reply message to "BUSY"
                m_service.setReplyMsg("BUSY");

                // listen and reply busy to testman
                result = m_service.monitor();
                if (result != SUCCESS) {
                    k_log->error(
                        "The flexiv system is having an error in "
                        "connection, wait for task to be finished...");
                    k_log->error(
                        "=================================================="
                        "=");
                    // if socket fails, wait for current task to finish and
                    // set status to fault
                    t_pool.join();
                    m_flexivStatus = FAULT;
                    return;
                }
                break;
            }

            case DONE: {
                k_log->info("===================================================");
                k_log->info("The flexiv system is turning off...");
                k_log->info("===================================================");
                // just close the socket service
                m_service.disconnect();
                k_log->info("Flexiv system server closed this connection safely");
                return;
            }

            // this status is actually not used, since socket error cause
            // return
            case FAULT: {
                m_service.setReplyMsg("FAULT");
                k_log->error("The flexiv system is in fault mode...");
                k_log->error("===================================================");
                // set reply content to "FAULT" and reply to client
                result = m_service.monitor();
                if (result != SUCCESS) {
                    k_log->error(
                        "The flexiv system is having an error in "
                        "fault status");
                    k_log->error(
                        "=================================================="
                        "=");
                    m_flexivStatus = FAULT;

                    return;
                }
                m_service.disconnect();
                k_log->error(
                    "Flexiv system server closed this connection due to "
                    "fault");
                return;
            }
        }
    }
}

bool Communications::checkSeriousErrorStatus()
{
    if (m_seriousErrorFlag == true) {
        return true;
    } else {
        return false;
    }
}

bool Communications::checkTerminateStatus()
{
    if (m_terminateSwitch == true) {
        return true;
    } else {
        return false;
    }
}

} /* namespace kostal */
