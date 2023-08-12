/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:23:50
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-09 18:00:26
 * @ Description:
 */


#include "FlexivLog.hpp"

namespace kostal {

FlexivLog * FlexivLog::getInstance()
{
    static FlexivLog FlexivLog;
    return &FlexivLog;
}

std::shared_ptr<spdlog::logger> FlexivLog::getLogger()
{
    return m_loggerPtr;
}

FlexivLog::FlexivLog():m_maxSize(g_maxSize),m_filesCount(g_filesCount),m_loggerName(g_loggerName)
{
    m_loggerDir = g_loggerDir + "FlexivLog" + getTime();
    m_loggerPtr = spdlog::rotating_logger_mt(m_loggerName, m_loggerDir, m_maxSize, m_filesCount);
    spdlog::flush_every(std::chrono::seconds(1));
}

FlexivLog::~FlexivLog()
{
    spdlog::drop_all();
}

void FlexivLog::info(const std::string& message) const
{
    m_loggerPtr.get()->info(message);
}

void FlexivLog::warn(const std::string& message) const
{
    m_loggerPtr.get()->warn(message);
}

void FlexivLog::error(const std::string& message) const
{
    m_loggerPtr.get()->error(message);
}

} /* namespace kostal */
