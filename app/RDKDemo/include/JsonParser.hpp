/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:08:59
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-11 18:03:23
 * @ Description:
 */

#ifndef FLEXIVRDK_JSON_PARSER_HPP_
#define FLEXIVRDK_JSON_PARSER_HPP_

#include "FlexivLog.hpp"
#include "SystemParams.hpp"

namespace kostal {

/**
 * @class JsonParser
 * @brief Base class for message to jsonalize or unjsonalize
 */
class JsonParser
{
private:
    // create a json formay reader
    Json::Reader m_jsonReader;

    // create a json value object for receiving values
    Json::Value m_jsonRecvValue;

    // create a json value object for sending values
    Json::Value m_jsonSendValue;

    // create a json style writter for generating json style content from json
    // value object
    Json::StyledWriter m_jsonStyledWriter;

public:
    JsonParser() = default;
    virtual ~JsonParser() = default;

    /**
     * @brief Parse the received message and take out the token
     * @param[in] recvMsg the received message that will be parsed
     * @param[out] recvToken the received token parsed from recvMsg
     * @param[in] logPtr kostal's log pointer
     * @return Status code
     */
    Status parseTokenJSON(std::string* recvMsg, std::string* recvToken);

    /**
     * @brief Parse the received message and take out key value for heartbeat
     * between this server and testman
     * @param[in] recvMsg the received message that will be parsed
     * @param[out] queryStatus whether client want to query status or not
     * @param[out] terminateStatus whether client want to terminate or not
     * @param[in] logPtr kostal's log pointer
     * @return Status code
     */
    Status parseHBJSON(
        std::string* recvMsg, std::string* queryStatus, std::string* terminateStatus);

    /**
     * @brief Parse the received message and take out key value for task
     * execution
     * @param[in] recvMsg the received message that will be parsed
     * @param[out] queryStatus whether testman want to query or not
     * @param[out] taskType the type of the work plan in message: NORMAL BIAS
     * DUMMY
     * @param[out] taskName the name of the workplan in message: xxx-MainPlan
     * @param[in] logPtr kostal's log pointer
     * @return Status code
     */
    Status parseTaskJSON(std::string* recvMsg, std::string* queryStatus, std::string* taskType,
        std::string* taskName);

    /**
     * @brief Generate message into JSON format before sending
     * @param[in] systemStatus the flexiv system status: IDLE BUSY FAULT
     * @param[out] sendMessage the message will be sent after jsonalizing
     * @param[in] logPtr kostal's log pointer
     * @return Status code
     */
    Status generateSendJSON(std::string* systemStatus, std::string* sendMessage);
};

} /* namespace kostal */

#endif /* FLEXIVRDK_JSON_PARSER_HPP_ */