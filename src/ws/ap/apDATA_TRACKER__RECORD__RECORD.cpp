#include "apDATA_TRACKER.h"

// apDATA_TRACKER::RECORD::RECORD @ 0x82A24810
// Default-construct: the `name` member adopts the shared empty-string singleton. The DB shows
// dsTSTRING<char>::UnsafeInitEmpty inlined here (lazy-init of the process-wide empty buffer,
// then share it and bump its refcount); modeled as the equivalent UnsafeInitEmpty call. The
// values[30] array is left uninitialised, matching the binary.
apDATA_TRACKER::RECORD::RECORD()
{
    this->name.pBuffer = nullptr;
    this->name.UnsafeInitEmpty();
}
