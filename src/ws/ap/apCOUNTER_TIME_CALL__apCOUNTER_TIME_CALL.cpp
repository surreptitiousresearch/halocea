#include "apCOUNTER_TIME_CALL.h"

// apCOUNTER_TIME_CALL::apCOUNTER_TIME_CALL @ 0x8271A930
apCOUNTER_TIME_CALL::apCOUNTER_TIME_CALL(const char *name)
    : apCOUNTER(name, "time/calls")
{
    this->customFreq = 0;
    this->__vftable = (apCOUNTER_vtbl *)&apCOUNTER_TIME_CALL::vftable;

    for (int i = 0; i < 8; ++i)
    {
        this->tmData[i].start = 0;
        this->tmData[i].sum = 0;
    }
    this->tmDataTotal.start = 0;
    this->tmDataTotal.sum = 0;
    this->callsTotal = 0;
}
