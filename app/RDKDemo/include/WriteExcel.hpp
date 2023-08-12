/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:08:59
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-11 22:39:51
 * @ Description:
 */

#ifndef FLEXIVRDK_WRITEEXCEL_HPP_
#define FLEXIVRDK_WRITEEXCEL_HPP_

#include "RobotStates.hpp"
#include "FlexivLog.hpp"

namespace kostal {

/**
 * @class WriteExcel
 * @brief Base class of writing different data into an excel file
 */
class WriteExcel
{
public:
    WriteExcel() = default;
    virtual ~WriteExcel() = default;

    /**
     * @brief Write the robot data list into a csv file
     * with associated name
     * @param[in] taskType the type of the task, can be NORMAL, BIAS, DUMMY
     * @param[in] taskName the name of the task
     * @param[in] robotDataListPtr robot data list's pointer
     * @return Status code
     */
    Status writeDataToExcel(
        std::string taskType, std::string taskName, std::list<RobotData>* robotDataListPtr);

public:
    int64_t firstTimestamp {0};
};

} /* namespace kostal */

#endif /* FLEXIVRDK_WRITEEXCEL_HPP_ */
