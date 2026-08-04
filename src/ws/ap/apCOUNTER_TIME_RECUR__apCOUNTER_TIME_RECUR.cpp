#include "apCOUNTER_TIME_RECUR.h"
#include "apPROFILER.h"

// apCOUNTER_TIME_RECUR::apCOUNTER_TIME_RECUR @ 0x8271AA98
apCOUNTER_TIME_RECUR::apCOUNTER_TIME_RECUR(const char *name, apPROFILER *prof)
    : apCOUNTER(name, "time")
{
    this->customFreq = 0;
    this->__vftable = (apCOUNTER_vtbl *)&apCOUNTER_TIME_RECUR::vftable;

    for (int i = 0; i < 8; ++i)
    {
        this->tmData[i].start = 0;
        this->tmData[i].sum = 0;
        this->tmData[i].depth = 0;
    }
    this->tmDataTotal.start = 0;
    this->tmDataTotal.sum = 0;
    this->tmDataTotal.depth = 0;

    if (prof)
    {
        apCOUNTER_UNIT &unit0 = this->counterUnits[0];
        unit0.myProfiler = prof;
        prof->currentUnit = &unit0;
        unit0.state.state |= 0x10u;
    }
}
