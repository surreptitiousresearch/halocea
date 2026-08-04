#include "../ap/apDATA_TRACKER.h"
#include "../ds/dsTSTRING_BUF_HEADER.h"
#include <string.h>

// Global placement copy-construct helper: construct a copy of *val at *place, returning place
// (or null if place is null). Declared as the primary template it specializes.
template<class T> T *New(T *place, const T *val);

// New<apDATA_TRACKER::RECORD> @ 0x82A245D0
// Placement copy-construct of a tracker RECORD: share the source name buffer (bump its refcount)
// and byte-copy the value array. No refcount is dropped on `place->name` first because the slot
// is raw (uninitialised) storage — pBuffer is cleared before the share.
template<>
apDATA_TRACKER::RECORD *New<apDATA_TRACKER::RECORD>(apDATA_TRACKER::RECORD *place,
                                                    const apDATA_TRACKER::RECORD *val)
{
    if (!place)
        return nullptr;

    place->name.pBuffer = nullptr;
    dsTSTRING_BUF_HEADER<char> *sharedBuffer = val->name.pBuffer;
    place->name.pBuffer = sharedBuffer;
    ++sharedBuffer->refCount;

    memcpy(place->values, val->values, sizeof(place->values));
    return place;
}
