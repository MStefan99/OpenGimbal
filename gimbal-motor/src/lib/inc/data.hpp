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
    extern const uint8_t& polePairs;
    
    void write(const uint8_t& addr, uint8_t& buf, uint8_t len = 1);
}

#endif	/* DATA_HPP */
