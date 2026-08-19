#include "headers/ws/vid/vidDIP.h"

// @0x8266A838 -- DEVIATION: the decompiler's `v1.file` (the apCL cookie's source-file pointer)
// gets clobbered by an unrelated refcount-bump on the shared empty-string singleton due to a
// stack-slot-reuse artifact, obscuring the literal. Inferred from the identical pattern in the
// adjacent vidDIP_CLASS::vidDIP_CLASS (0x8266A92C, same file/header, cookie line 454) which shows
// the literal directly: "D:\\Projects\\code\\common\\incl.sys\\DRV\\vid.h".
vidDIP::vidDIP()
    : dipIDList(apCL{ "D:\\Projects\\code\\common\\incl.sys\\DRV\\vid.h", 439 })
{
    this->dipIdCount = 0;
    this->dipIdMax = 0;
    this->polyIdMax = 0;
    this->name.UnsafeInitEmpty();

    this->counter.dipCur = 0;
    this->counter.dipMax = 0;
    this->counter.polyCur = 0;
    this->counter.polyMax = 0;
    this->counter.dip = 0;
    this->counter.poly = 0;
}
