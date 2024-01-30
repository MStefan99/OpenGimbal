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
    
    enum class VID: uint8_t {
        Mode = 0x0,
        Battery = 0x1,
        TempController = 0x2,
        TempMotors = 0x3,
        Error = 0xf
    };
    
    void init();
    void sleep();
    void calibrate();
    template <class T>
    void getVariable(uint8_t vid);
    template <class T>
    void setVariable(uint8_t vid, T value);
}


template <class T>
void main_board::getVariable(uint8_t vid) {
    uint8_t buf[2] = {
        static_cast<uint8_t>(OutgoingCommand::Calibrate) | 2 << 4u,
        vid
    };
    uart::send(buf, 2);
}


template <class T>
void main_board::setVariable(uint8_t vid, T value) {
    uint8_t len = sizeof(T) + 2;
    uint8_t buf[len] = {
        static_cast<uint8_t>(OutgoingCommand::Calibrate) | len << 4u,
        vid
    };
    util::copy(buf + 2, value, sizeof(value));
    uart::send(buf, len);
}

#endif	/* MAIN_BOARD_HPP */

