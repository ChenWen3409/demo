/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:08:59
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-11 18:10:06
 * @ Description:
 */

#ifndef FLEXIVRDK_ROBOTOPERATIONS_HPP_
#define FLEXIVRDK_ROBOTOPERATIONS_HPP_

#include "RobotStates.hpp"
#include "FlexivLog.hpp"
namespace kostal {

/**
 * @class RobotOperations
 * @brief Base class for robot operations, dealt by a handler
 */
class RobotOperations
{
public:
    RobotOperations() = default;
    virtual ~RobotOperations() = default;

    /**
     * @brief Check robot connection and set robot to plan execution mode
     * @param[in] robotPtr robot's pointer
     * @return Status code
     */
    Status buildRobotConnection(flexiv::Robot* robotPtr);

    /**
     * @brief This function helps to clear some tiny software fault that users
     * trigger if return is not success, it means the error can not be
     * eliminated
     * @param[in] robotPtr Pointer to robot object
     * @return Flexiv status code
     */
    Status clearTinyFault(flexiv::Robot* robotPtr);

    /**
     * @brief Access the current robot and spi data and store it into each list
     * @param[in]  robotPtr robot's pointer
     * @param[out] robotDataPtr robot data's pointer
     * @param[out] robotDataListPtr robot data list's pointer
     * @return Status code
     */
    Status collectSyncData(
        flexiv::Robot* robotPtr, RobotData* robotDataPtr, std::list<RobotData>* robotDataListPtr);

    /**
     * @brief Check whether robot has this plan in list
     * @param[in] robotPtr robot's pointer
     * @param[in] planName the name of the executing work plan
     * @return yes for do have, no for do not have
     */
    bool checkRobotPlan(flexiv::Robot* robotPtr, std::string planName);

    /**
     * @brief Execute the plan in robot by name
     * @param[in] robotPtr robot's pointer
     * @param[in] planName the name of the executing work plan
     * @return Status code
     */
    Status executeRobotPlan(flexiv::Robot* robotPtr, std::string planName);
};

} /* namespace kostal */

#endif /* FLEXIVRDK_ROBOTOPERATION_HPP_ */
