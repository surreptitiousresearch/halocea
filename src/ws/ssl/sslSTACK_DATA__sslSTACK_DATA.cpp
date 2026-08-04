#include "sslSTACK_DATA.h"

// sslSTACK_DATA::sslSTACK_DATA(const sslSTACK_DATA &val) @ 0x82AF0CD0
// Copy-construct: default-init the (uninitialised) slot to TYPE_INT/0 first (so operator= always
// sees a well-defined, non-ref-counted prior payload to drop), then delegate to operator=.
sslSTACK_DATA::sslSTACK_DATA(const sslSTACK_DATA &val)
{
    this->type = TYPE_INT;
    this->int_val = 0;
    *this = val;
}
