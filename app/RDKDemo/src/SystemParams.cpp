/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:23:50
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-23 15:46:10
 * @ Description:
 */

#include "SystemParams.hpp"

const unsigned short g_COMMPORT = 6070;

std::string ROBOTADDRESS = "192.168.2.100";

std::string LOCALADDRESS = "192.168.2.101";

const int g_MSGMAXSIZE = 1024;

const std::string g_TOKEN = "flexiv";

const std::string UPLOADADDRESS = "/home/chenwen/ftp/";

std::mutex g_kostalDataMutex;

const std::string TOKEN = "TOKEN";

const std::string QUERYSTATUS = "TM_FLEXIV_QUERY_STATUS";

const std::string TASKTYPE = "TM_FLEXIV_TASK_TYPE";

const std::string TASKNAME = "TM_FLEXIV_TASK_NAME";

const std::string TERMINATE = "TM_FLEXIV_TERMINATE";

const std::string SYSTEMSTATUS = "FLEXIV_TM_STATUS";

std::atomic<bool> g_dataCollectFlag = {false};

std::atomic<bool> g_collectSwitch = {false};

int64_t g_timeoutInterval = 5;

std::string g_loggerName = "FlexivLog";

std::string g_loggerDir = "/home/chenwen/logfile/";

const int g_maxSize = 10 * 1024 * 1024;

const int g_filesCount = 100;