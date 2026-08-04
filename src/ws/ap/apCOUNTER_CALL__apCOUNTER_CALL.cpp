#include "apCOUNTER_CALL.h"

// apCOUNTER_CALL::apCOUNTER_CALL @ 0x8271AE30
apCOUNTER_CALL::apCOUNTER_CALL(const char *name)
    : apCOUNTER(name, "calls")
{
    this->__vftable = (apCOUNTER_vtbl *)&apCOUNTER_CALL::vftable;
}
