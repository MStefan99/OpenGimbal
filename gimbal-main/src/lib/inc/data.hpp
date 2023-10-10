/* 
 * File:   data.hpp
 * Author: Mikhail
 *
 * Created on June 22, 2023, 17:58 PM
 */

#ifndef DATA_HPP
#define	DATA_HPP

#include "device.h"
#include <cstdlib>

#include "lib/inc/util.hpp"


namespace data {    
    extern const uint8_t NVM_DATA[FLASH_PAGE_SIZE * 4];
    
    typedef struct __attribute__((packed)) {
        uint8_t polePairs;
        int8_t direction;
        uint16_t zeroOffset;
    } Options;
    
    extern const Options& options;
    
    void edit(const uint8_t& addr, uint8_t& buf, uint8_t len = 1);
    void write();
}

#endif	/* DATA_HPP */
