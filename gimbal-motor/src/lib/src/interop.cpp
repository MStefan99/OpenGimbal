#include "lib/inc/interop.h"
#include "lib/inc/bldc.hpp"

extern "C" {
    void bldcDisable() {
        bldc::disable();
    }
}
