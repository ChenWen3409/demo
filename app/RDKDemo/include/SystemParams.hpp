/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:08:59
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-10 10:55:33
 * @ Description:
 */

#ifndef FLEXIVRDK_SYSTEMPARAMS_HPP_
#define FLEXIVRDK_SYSTEMPARAMS_HPP_

// Flexiv header files
#include <flexiv/Exception.hpp>
#include <flexiv/Gripper.hpp>
#include <flexiv/Log.hpp>
#include <flexiv/Mode.hpp>
#include <flexiv/Model.hpp>
#include <flexiv/Robot.hpp>
#include <flexiv/Scheduler.hpp>
#include <flexiv/Data.hpp>
#include <flexiv/Utility.hpp>

// STL header files
#include <iostream>
#include <string>
#include <cstring>
#include <mutex>
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include <thread>
#include <functional>
#include <iomanip>
#include <fstream>
#include <future>
#include "math.h"
#include <chrono>


// 3rd-party libs
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/thread_pool.hpp>
#include <jsoncpp/json/json.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <eigen3/Eigen/Eigen>

// The port number of destination host
extern const unsigned short g_COMMPORT;

// Robot IP Address
extern std::string ROBOTADDRESS;

// Local IP Address
extern std::string LOCALADDRESS;

// Max length of the socket message
extern const int g_MSGMAXSIZE;

// Token for shake-hand with kostal testman
extern const std::string g_TOKEN;

// The file stored location prefix
extern const std::string UPLOADADDRESS;

// The global variant status shows the status of the system, 0 means success,
// 1-4 means errors in different periods
enum Status
{
    SUCCESS,
    SOCKET,
    JSON,
    ROBOT,
    CSV,
    FTP,
    SYSTEM
};

// The status of the flexiv server
enum serverStatus
{
    INIT,
    IDLE,
    BUSY,
    FAULT,
    DONE
};

// global mutext for spi data collection and robot data collection
extern std::mutex g_kostalDataMutex;

// Json message key of query status
extern const std::string TOKEN;

// Json message key of query status
extern const std::string QUERYSTATUS;

// Json message key of task type
extern const std::string TASKTYPE;

// Json message key of task name
extern const std::string TASKNAME;

// Json message key of terminate status
extern const std::string TERMINATE;

// Json message key of flexiv system status
extern const std::string SYSTEMSTATUS;

// Whether the node data should be collected or not
extern std::atomic<bool> g_dataCollectFlag;

// Whether the whole collecting logic should be used or not
extern std::atomic<bool> g_collectSwitch;

// Connection timeout interval after first handshake with Testman, unit is second
extern int64_t g_timeoutInterval;

// logger setting
extern std::string g_loggerName;
extern std::string g_loggerDir;
extern const int g_maxSize;
extern const int g_filesCount;

#endif /* FLEXIVRDK_SYSTEMPARAMS_HPP_ */