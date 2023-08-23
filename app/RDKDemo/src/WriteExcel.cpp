/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:23:50
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-23 17:15:34
 * @ Description:
 */

#include "WriteExcel.hpp"

namespace kostal {

// get current time in time_t format
std::string getTime()
{
    time_t timep;
    time(&timep); // get current time in time_t type
    char strTime[64];
    strftime(strTime, sizeof(strTime), "_%Y-%m-%d_%H:%M:%S", localtime(&timep));
    return strTime;
}

// transfer a double array of tcp quaternion to an array of euler
std::array<double, 3> quaternionToEuler(double* tcpPose)
{
    double M_Pi;
    double rad = 180 / M_PI;
    Eigen::Quaternion<double> q;
    std::array<double, 3> eulerAngle;
    q.w() = tcpPose[0];
    q.x() = tcpPose[1];
    q.y() = tcpPose[2];
    q.z() = tcpPose[3];
    auto euler = q.toRotationMatrix().eulerAngles(2, 1, 0);
    eulerAngle[0] = euler[2];
    eulerAngle[1] = euler[1];
    eulerAngle[2] = euler[0];
    return eulerAngle;
}

Status WriteExcel::writeDataToExcel(
    std::string taskType, std::string taskName, std::list<RobotData>* robotDataListPtr)
{

    if (robotDataListPtr->size() == 0) {
        k_log->error("The collected robot data list is null, exiting...");
        return CSV;
    }

    std::fstream excelFile;
    std::string filePath = UPLOADADDRESS;
    std::string excelFileName = filePath + taskType + "/" + taskName + getTime() + ".csv";
    std::cout << "The generated file path is: " << excelFileName << std::endl;
    excelFile.open(excelFileName, std::ios::out);
    if (!excelFile.is_open()) {
        k_log->error("The associated excel file is not created correctly");
        return CSV;
    }

    // Header
    excelFile << "NodeName"
              << ","
              << "Index"
              << ",";
    excelFile << "TCP_x"
              << ","
              << "TCP_y"
              << ","
              << "TCP_z"
              << ",";
    excelFile << "TCP_Rx"
              << ","
              << "TCP_Ry"
              << ","
              << "TCP_Rz"
              << ",";
    excelFile << "FLANGE_x"
              << ","
              << "FLANGE_y"
              << ","
              << "FLANGE_z"
              << ",";
    excelFile << "FLANGE_Rx"
              << ","
              << "FLANGE_Ry"
              << ","
              << "FLANGR_Rz"
              << ",";
    excelFile << "RawDataSensor0"
              << ","
              << "RawDataSensor1"
              << ","
              << "RawDataSensor2"
              << ",";
    excelFile << "RawDataSensor3"
              << ","
              << "RawDataSensor4"
              << ","
              << "RawDataSensor5"
              << ","
              << "timestamp" << std::endl;

    std::list<RobotData>::iterator robotIterator = robotDataListPtr->begin();
    for (int index = 0; index < robotDataListPtr->size(); index++) {
        // node name
        excelFile << (*robotIterator).nodeName << ",";

        // Index increase
        excelFile << std::to_string(index) << ",";

        // tcp xyz
        excelFile << (*robotIterator).tcpPose[0] << ",";
        excelFile << (*robotIterator).tcpPose[1] << ",";
        excelFile << (*robotIterator).tcpPose[2] << ",";

        // tcp euler data
        double quaternionTcp[4] = {(*robotIterator).tcpPose[3], (*robotIterator).tcpPose[4],
            (*robotIterator).tcpPose[5], (*robotIterator).tcpPose[6]};
        auto eulerTcp = quaternionToEuler(quaternionTcp);
        excelFile << eulerTcp[0] << ",";
        excelFile << eulerTcp[1] << ",";
        excelFile << eulerTcp[2] << ",";

        // flange xyz
        excelFile << (*robotIterator).flangePose[0] << ",";
        excelFile << (*robotIterator).flangePose[1] << ",";
        excelFile << (*robotIterator).flangePose[2] << ",";

        // flange euler data
        double quaternionFlange[4]
            = {(*robotIterator).flangePose[3], (*robotIterator).flangePose[4],
                (*robotIterator).flangePose[5], (*robotIterator).flangePose[6]};
        auto eulerFlange = quaternionToEuler(quaternionFlange);
        excelFile << eulerFlange[0] << ",";
        excelFile << eulerFlange[1] << ",";
        excelFile << eulerFlange[2] << ",";

        // raw sensor data
        for (int i = 0; i < 6; i++) {
            excelFile << (*robotIterator).rawDataForceSensor[i] << ",";
        }
        excelFile << std::to_string((*robotIterator).timestamp);
        excelFile << std::endl;
        robotIterator++;
    }

    excelFile.close();
    return SUCCESS;
}

} /* namespace kostal */
