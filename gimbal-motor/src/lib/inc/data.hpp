/* 
 * File:   data.hpp
 * Author: Mikhail
 *
 * Created on June 22, 2023, 17:58 PM
 */

#ifndef DATA_HPP
#define	DATA_HPP

#include "device.h"

#include "lib/inc/Matrix.hpp"
#include "lib/inc/util.hpp"


namespace data {    
    extern const uint8_t NVM_DATA[FLASH_PAGE_SIZE * 4];
//    extern const uint8_t& poles;
    
    void save();
}

#endif	/* DATA_HPP */
