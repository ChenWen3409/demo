/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:23:50
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-09 21:34:55
 * @ Description:
 */

#include "RobotOperations.hpp"

namespace kostal {

Status RobotOperations::buildRobotConnection(flexiv::Robot* robotPtr)
{
    // Check whether the robot is connected or not
    if (robotPtr->isConnected() != true) {
        k_log->error("The robot is not connected");
        return ROBOT;
    } else
        std::cout << "The robot is now connected" << std::endl;

    // Clear fault on robot server if any
    if (robotPtr->isFault()) {
        k_log->warn("Robot is facing a fault, trying to clear the fault...");

        // Try to clear the fault
        robotPtr->clearFault();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Check fault again
        if (robotPtr->isFault()) {
            k_log->error(
                        "===================================================");
            k_log->error("Robot's fault cannot be cleared, exiting ...");

            return ROBOT;
        }
        k_log->info("Fault on robot server is cleared");
    }

    // Enable the robot, make sure the E-stop is released before enabling
    std::cout << "Enabling robot ..." << std::endl;
    robotPtr->enable();

    // Wait for the robot to become operational
    while (!robotPtr->isOperational()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "The robot is now operational" << std::endl;

    // Set mode after robot is operational
    robotPtr->setMode(flexiv::Mode::NRT_PLAN_EXECUTION);
    
    // Wait for the mode to be switched
    while (robotPtr->getMode() != flexiv::Mode::NRT_PLAN_EXECUTION) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "The robot is now in plan execution mode" << std::endl;

    return SUCCESS;
}

Status RobotOperations::clearTinyFault(flexiv::Robot* robotPtr)
{
    // Clear fault on robot server if any
    if (robotPtr->isFault()) {
        k_log->warn("Robot is facing a fault, trying to clear the fault...");
        // Try to clear the fault
        robotPtr->clearFault();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        // Check again
        if (robotPtr->isFault()) {
            k_log->error(
                        "===================================================");
            k_log->error("Robot's fault cannot be cleared, exiting ...");
            return ROBOT;
        } else {
            std::cout << "Fault on robot server is cleared" << std::endl;
        }
    }

    return SUCCESS;
}

Status RobotOperations::collectSyncData(flexiv::Robot* robotPtr,
                                        RobotData* robotDataPtr, std::list<RobotData>* robotDataListPtr)
{
    while (g_collectSwitch) {
        // get plan info
        flexiv::PlanInfo planInfo;
        robotPtr->getPlanInfo(planInfo);

        // start collecting when nodename is "Start"
        if (planInfo.nodeName == "Start") {
            g_dataCollectFlag = true;
        }

        // stop collecting when nodename is "Stop"
        if (planInfo.nodeName == "Stop") {
            g_dataCollectFlag = false;
        }

        // only collect when collect flag is on
        if (g_dataCollectFlag == true) {
            robotDataPtr->nodeName = planInfo.nodeName;

            // get robot states
            flexiv::RobotStates robotStates;
            {
                // get robot states and input them to robot data instance
                robotPtr->getRobotStates(robotStates);
                robotDataPtr->tcpPose = robotStates.tcpPose;
                robotDataPtr->rawDataForceSensor = robotStates.ftSensorRaw;
                robotDataPtr->flangePose = robotStates.flangePose;
                robotDataPtr->timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

                // put data instance to list
                robotDataListPtr->push_back(*robotDataPtr);
            }
        }
    }
    std::cout << "robot collect finish!" << std::endl;

    return SUCCESS;
}

bool RobotOperations::checkRobotPlan(
        flexiv::Robot* robotPtr, std::string planName)
{
    auto planList = robotPtr->getPlanNameList();

    if (std::find(planList.begin(), planList.end(), planName)
            == planList.end()) {
        k_log->error("=================================================");
        k_log->error("The robot does not have planName: " + planName);
        return false;
    }
    // if exists, return true
    return true;
}

Status RobotOperations::executeRobotPlan(
        flexiv::Robot* robotPtr, std::string planName)
{
    // execute plan by name
    robotPtr->executePlan(planName);

    // sleep when robot is prepare to execute
    while (robotPtr->isBusy()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return SUCCESS;
}

} /* namespace kostal */
