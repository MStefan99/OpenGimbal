/*
 * File:   data.hpp
 * Author: Mikhail
 *
 * Created on June 22, 2023, 17:58 PM
 */

#ifndef NVM_HPP
#define NVM_HPP

#include <cstdlib>

#include "device.h"

#include "options.hpp"
#include "util.hpp"


#define FLASH_ROW_SIZE (FLASH_PAGE_SIZE * 4)

/* Non-volatile memory controller can write one page and erase one row (4 pages) at a time,
 * therefore, a following procedure is used:
 * - Upon any edits, a copy of an entire row is made
 * - Multiple edits can take place
 * - After calling write() or when starting to edit another row, the current row is erased,
 * the data is copied back and all 4 pages are written.
 */


namespace nvm {
	struct __attribute__((packed)) Options {
		uint8_t  polePairs {0};
		bool     counterclockwise {true};
		uint16_t phaseOffset {0};
		uint16_t zeroOffset {0};
		uint8_t  maxSpeed {0};
	};

	namespace _internal {
		extern uint8_t        rowCopy[FLASH_ROW_SIZE];  // Copy of the row
		extern const uint8_t* modifiedRow;              // Original address of the data

		struct __attribute__((packed)) Rows {
			union {
				Options options {};
				uint8_t pad[FLASH_ROW_SIZE];
			};
		};

		extern const Rows rows;
	}

	extern const Options* options;

	void write();

	template <class T>
	void edit(const T* dest, T src) {
		// Cannot use % with pointers, need to cast to a number and back
		auto row {reinterpret_cast<const uint8_t*>(
		    reinterpret_cast<uint32_t>(dest) - (reinterpret_cast<uint32_t>(dest) % FLASH_ROW_SIZE)
		)};

		if (reinterpret_cast<const _internal::Rows*>(row) < &_internal::rows
		    || reinterpret_cast<const _internal::Rows*>(row) >= &_internal::rows + 1) {
			return;
		}

		if (reinterpret_cast<const uint8_t*>(row) != _internal::modifiedRow) {  // Starting to edit another row
			write();                                                              // Writing current row
			util::copy(                                                           // Making a copy of the new row
			    reinterpret_cast<uint32_t*>(_internal::rowCopy),                  // Destination
			    reinterpret_cast<const uint32_t*>(row),                           // New row
			    FLASH_ROW_SIZE / sizeof(uint32_t)  // Copying the entire row in 32-bit operations
			);
			_internal::modifiedRow = row;
		}

		*reinterpret_cast<T*>(_internal::rowCopy + (reinterpret_cast<const uint8_t*>(dest) - row)) = src;
	}
}  // namespace nvm

#endif /* NVM_HPP */
