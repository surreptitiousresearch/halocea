#include "apDATA_TRACKER.h"
#include "../os/osLOCK.h"

// apDATA_TRACKER::AddValue @ 0x82A25558
// Accumulate `value` into the current frame slot of the named series (create it on first use).
void apDATA_TRACKER::AddValue(const char *name, float value)
{
    this->lock.Lock(nullptr, 0);

    int recordIndex = this->FindAdd(name);
    float &slot = this->records[recordIndex].values[this->frameCounter];
    slot = slot + value;

    this->lock.Unlock(nullptr, 0);
}
