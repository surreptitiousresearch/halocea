#include "apCOUNTER_TRACKER.h"

// apCOUNTER_TRACKER::apCOUNTER_TRACKER @ 0x82715088
//
// CAVEAT: the raw decompile flags "write access to const memory has been detected" and inlines
// the local-static-guard body of dsTSTRING<char>::UnsafeInitEmpty() directly into this ctor
// (constructing the process-wide shared empty-string singleton on first use). Reconstructed here
// as a call to that existing method, which is semantically identical and avoids duplicating the
// guarded-static machinery.
apCOUNTER_TRACKER::apCOUNTER_TRACKER()
{
    this->started = false;
    this->stopped = false;
    this->nFrameAll = 0;
    this->cntAdded = false;

    this->cntFileName.pBuffer = nullptr;
    this->cntFileName.UnsafeInitEmpty();

    this->eventLog.items.pData = nullptr;
    this->eventLog.items.nElem = 0;
    this->eventLog.items.allocated = 0;
    this->eventLog.items.__cl.file = "D:\\Projects\\code\\common\\src.sys\\ap\\ap_prof_counters.cpp";
    this->eventLog.items.__cl.line = 1517;
    this->eventLog.sizeText = 0;
}
