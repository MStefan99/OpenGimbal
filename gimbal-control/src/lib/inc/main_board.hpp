/* 
 * File:   main_board.hpp
 * Author: Mikhail
 *
 * Created on January 30, 2024, 9:36 AM
 */

#ifndef MAIN_BOARD_HPP
#define	MAIN_BOARD_HPP

#include "device.h"

#include "uart.hpp"


namespace main_board {
    enum class OutgoingCommand: uint8_t {
        Sleep = 0x0,
        Move = 0x1,
        Calibrate = 0x2,
        GetVariable = 0x3,
        SetVariable = 0x4
    };
    
    enum class IncomingCommand: uint8_t {
        GetVariable = 0x0
    };
    
    void init();
    void sleep();
    void calibrate();
}

#endif	/* MAIN_BOARD_HPP */

