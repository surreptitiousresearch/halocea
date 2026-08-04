#include "dsDATA.h"

// Global placement copy-construct helper: construct a copy of *val at *place, returning place
// (or null if place is null). Declared as the primary template it specializes (see also
// New_apDATA_TRACKER_RECORD.cpp for the sibling instantiation).
template<class T> T *New(T *place, const T *val);

// New<dsDATA> @ 0x825B0D50
// Placement copy-construct a dsDATA: mark the destination empty (type = null) so the subsequent
// StoreValue sees an empty target and copy-constructs (rather than copy-assigns) from `*val`.
template<>
dsDATA *New<dsDATA>(dsDATA *place, const dsDATA *val)
{
    if (!place)
        return nullptr;

    place->type = nullptr;
    place->StoreValue(*val);
    return place;
}
