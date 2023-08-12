/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:08:59
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-11 18:00:58
 * @ Description:
 */

#ifndef FLEXIVRDK_FlexivLog_HPP_
#define FLEXIVRDK_FlexivLog_HPP_

#include "SystemParams.hpp"
#define k_log kostal::FlexivLog::getInstance()
namespace kostal {

/**
 * @class FlexivLog
 * @brief Loggers to Write messages in logfile with timestamp
 */

// get current time in time_t format
extern std::string getTime();

class FlexivLog
{
public:
    /**
     * @brief Get FlexivLog instance by only header
     * @return FlexivLog instance
     */
    static FlexivLog* getInstance();

    /**
     * @brief Get FlexivLog pointer of rotating_logger_mt
     * @return FlexivLog pointer
     */
    std::shared_ptr<spdlog::logger> getLogger();

    /**
     * @brief Write info message with timestamp
     * @param[in] message Info message
     */
    void info(const std::string& message) const;

    /**
     * @brief Write warning message with timestamp
     * @param[in] message Warning message
     */
    void warn(const std::string& message) const;

    /**
     * @brief Write error message with timestamp
     * @param[in] message Error message
     */
    void error(const std::string& message) const;

private:
    FlexivLog();
    virtual ~FlexivLog();
    FlexivLog(const FlexivLog&) = delete;
    FlexivLog& operator=(const FlexivLog&) = delete;

    // creat spdlog pointer
    std::shared_ptr<spdlog::logger> m_loggerPtr;

    // log name
    std::string m_loggerName;

    // log direction
    std::string m_loggerDir;

    // the max size of logfile
    const int m_maxSize;

    // the counter of all logfiles
    const int m_filesCount;
};

} /* namespace kostal */

#endif /* FLEXIVRDK_FlexivLog_HPP_ */
