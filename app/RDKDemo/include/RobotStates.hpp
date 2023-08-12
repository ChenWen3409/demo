/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:08:59
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-10 00:26:59
 * @ Description:
 */

#ifndef FLEXIVRDK_KOSTALSTATES_HPP_
#define FLEXIVRDK_KOSTALSTATES_HPP_

#include "SystemParams.hpp"

namespace kostal {

/**
 * @class RobotData
 * @brief The class of the robot data type
 */
class RobotData
{
public:
    RobotData() = default;
    virtual ~RobotData() = default;
    std::string nodeName;
    std::vector<double> tcpPose; // [7]
    std::vector<double> flangePose; // [7]
    std::vector<double> rawDataForceSensor; // [6]
    int64_t timestamp{0};

public:
    // copy constructor
    RobotData(const RobotData& r)
    {
        nodeName = r.nodeName;
        tcpPose = r.tcpPose;
        flangePose = r.flangePose;
        rawDataForceSensor = r.rawDataForceSensor;
        timestamp = r.timestamp;
    }
};

} /* namespace kostal */

#endif /*FLEXIVRDK_KOSTALSTATES_HPP_*/
