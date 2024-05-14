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


#define FLASH_ROW_SIZE (FLASH_PAGE_SIZE * 4)

/* Non-volatile memory controller can write one page and erase one row (4 pages) at a time,
 * therefore, a following procedure is used: 
 * - Upon any edits, a copy of an entire row is made
 * - Multiple edits can take place
 * - After calling write() or when starting to edit another row, the current row is erased,
 * the data is copied back and all 4 pages are written.
 */


namespace data {
    namespace _internal {
        extern uint8_t rowCopy[FLASH_ROW_SIZE]; // Copy of the row
        extern const uint8_t* modifiedRow; // Original address of the data
    }
    
    typedef struct __attribute__((packed)) {
        union {
            struct {
            };
            uint8_t pad[FLASH_ROW_SIZE];
        };
    } Options;
    
    extern const Options options;
    
    void write();
    
    template <class T>
    void edit(const T* dest, T src) {
        // Cannot use % with pointers, need to cast to a number and back
        auto row {reinterpret_cast<const uint8_t*>(reinterpret_cast<uint32_t>(dest) - (reinterpret_cast<uint32_t>(dest) % FLASH_ROW_SIZE))};

        if (reinterpret_cast<const Options*>(row) < &options || reinterpret_cast<const Options*>(row) >= &options + 1) {
            return;
        }

        if (reinterpret_cast<const uint8_t*>(row) != _internal::modifiedRow) { // Starting to edit another row
            write(); // Writing current row
            util::copy( // Making a copy of the new row
                    reinterpret_cast<uint32_t*>(_internal::rowCopy), // Destination
                    reinterpret_cast<const uint32_t*>(row), // New row
                    FLASH_ROW_SIZE / sizeof(uint32_t) // Copying the entire row in 32-bit operations
            );
            _internal::modifiedRow = row;
        }

        *reinterpret_cast<T*>(_internal::rowCopy + (reinterpret_cast<const uint8_t*>(dest) - row)) = src;
    }
}

#endif	/* DATA_HPP */
