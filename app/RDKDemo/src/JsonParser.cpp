/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:23:50
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-23 15:31:04
 * @ Description:
 */
#include "JsonParser.hpp"

namespace kostal {

Status JsonParser::parseTokenJSON(std::string* recvMsg, std::string* recvToken)
{
    // if the received message is null
    if (recvMsg == nullptr || recvMsg->size() == 0) {
        k_log->error("The received json message is empty");
        return JSON;
    }

    // if the json reader is failed
    bool result;
    result = m_jsonReader.parse(*recvMsg, m_jsonRecvValue);
    if (!result) {
        k_log->error("The received message is not json format");
        return JSON;
    }

    // check the token first
    if (m_jsonRecvValue.isMember(TOKEN.c_str()) == 0) {
        k_log->error("The received json message does not have initialization param: " + TOKEN);
        return JSON;
    }

    // Retrieve the key value from init json message
    *recvToken = m_jsonRecvValue[TOKEN].asString();

    return SUCCESS;
}

Status JsonParser::parseHBJSON(
    std::string* recvMsg, std::string* queryStatus, std::string* terminateStatus)
{
    // if the received message is null
    if (recvMsg == nullptr || recvMsg->size() == 0) {
        k_log->error("The received json message is empty");
        return JSON;
    }

    // if the json reader is failed
    bool result;
    result = m_jsonReader.parse(*recvMsg, m_jsonRecvValue);
    if (!result) {
        k_log->error("The received message is not json format");
        return JSON;
    }

    // Check whether json has associated members
    if (m_jsonRecvValue.isMember(QUERYSTATUS.c_str()) == 0) {
        k_log->error("The received json message does not have param: " + QUERYSTATUS);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(TERMINATE.c_str()) == 0) {
        k_log->error("The received json message does not have param: " + TERMINATE);
        return JSON;
    }

    // Try to retrieve the key value from json
    *queryStatus = m_jsonRecvValue[QUERYSTATUS].asString();
    *terminateStatus = m_jsonRecvValue[TERMINATE].asString();

    // This can be all commented since it is for shakehand
    std::cout << "****************************************************" << std::endl;
    std::cout << "Check Message received!" << std::endl;
    std::cout << "The received queryStatus is: " + *queryStatus << std::endl;
    std::cout << "The received terminateStatus is: " + *terminateStatus << std::endl;
    std::cout << "****************************************************" << std::endl;

    return SUCCESS;
}

Status JsonParser::parseTaskJSON(
    std::string* recvMsg, std::string* queryStatus, std::string* taskType, std::string* taskName)
{
    // if the received message is null
    if (recvMsg == nullptr || recvMsg->size() == 0) {
        k_log->error("The received json message is empty");
        return JSON;
    }

    // if the json reader is failed
    bool result;
    result = m_jsonReader.parse(*recvMsg, m_jsonRecvValue);
    if (!result) {
        k_log->error("The received message is not json format");
        return JSON;
    }

    // Check whether json has associated members
    if (m_jsonRecvValue.isMember(QUERYSTATUS.c_str()) == 0) {
        k_log->error("The received json message does not have param: " + QUERYSTATUS);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(TASKTYPE.c_str()) == 0) {
        k_log->error("The received json message does not have param: " + TASKTYPE);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(TASKNAME.c_str()) == 0) {
        k_log->error("The received json message does not have param: " + TASKNAME);
        return JSON;
    }

    // Try to retrieve the key value from json
    *queryStatus = m_jsonRecvValue[QUERYSTATUS].asString();
    *taskType = m_jsonRecvValue[TASKTYPE].asString();
    *taskName = m_jsonRecvValue[TASKNAME].asString();

    std::cout << "****************************************************" << std::endl;
    std::cout << "Task message received!" << std::endl;
    std::cout << "The received taskType is: " + *taskType << std::endl;
    std::cout << "The received taskName is: " + *taskName << std::endl;
    std::cout << "****************************************************" << std::endl;

    return SUCCESS;
}

Status JsonParser::generateSendJSON(std::string* systemStatus, std::string* sendMessage)
{
    if (systemStatus == nullptr || systemStatus->size() == 0) {
        k_log->warn("The system status is empty");
        return JSON;
    }
    m_jsonSendValue[SYSTEMSTATUS] = *systemStatus;
    *sendMessage = m_jsonStyledWriter.write(m_jsonSendValue);
    std::cout << "The sending json message is: " + *sendMessage << std::endl;

    return SUCCESS;
}

} /* namespace kostal */