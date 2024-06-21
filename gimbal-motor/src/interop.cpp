#include "interop.h"
#include "bldc.hpp"

extern "C" {
    void bldcDisable() {
        bldc::disable();
    }
}
