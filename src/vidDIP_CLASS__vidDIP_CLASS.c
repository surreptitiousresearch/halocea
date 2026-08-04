#include "headers/ws/vid/vidDIP.h"

// 0x8266A92C
vidDIP_CLASS::vidDIP_CLASS()
    : dipList(apCL{ "D:\\Projects\\code\\common\\incl.sys\\DRV\\vid.h", 454 })
{
    this->name.UnsafeInitEmpty();

    this->counter.dipCur = 0;
    this->counter.dipMax = 0;
    this->counter.polyCur = 0;
    this->counter.polyMax = 0;
    this->counter.dip = 0;
    this->counter.poly = 0;
}
