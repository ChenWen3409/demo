/**
 * @ Author: wen chen
 * @ Create Time: 2023-08-09 16:23:50
 * @ Modified by: wen chen
 * @ Modified time: 2023-08-12 15:53:32
 * @ Description:
 */

// Kostal header files
#include "Communications.hpp"

int main()
{
    Status result;
    // we check robot connection and set robot to plan execution mode
    flexiv::Robot robot(ROBOTADDRESS, LOCALADDRESS);

    // put this process into a loop
    while (true) {
        // Instantiation of the com object
        kostal::Communications com;
        // 1st Initialize the server
        result = com.init(&robot);
        if (result == SYSTEM) {
            // failed to initialize, maybe robot, jump to stateMachine
        } else if (result == SUCCESS) {
            // initialize successfully, jump to stateMachine
        } else {
            // if socket failed, directly break
            break;
        }
        // 2nd Put the flexiv system into a state machine
        com.stateMachine(&robot);
        // 3rd Check whether system is terminated by client
        if (com.checkTerminateStatus()) {
            break;
        }
        // 4th Check whether system meet a fault in loop
        if (com.checkSeriousErrorStatus()) {
            break;
        }
    }
}