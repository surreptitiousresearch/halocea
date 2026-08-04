#include "sslSTACK_DATA.h"

// Global placement copy-construct helper: declared as the primary template it specializes
// (same convention as New_apDATA_TRACKER_RECORD.cpp).
template<class T> T *New(T *place, const T *val);

// New<sslSTACK_DATA> @ 0x82AF0D70
// Placement-copy-construct helper: default-init *place to TYPE_INT/0 then copy-assign `val` into
// it (mirrors the copy constructor's own body -- likely inlined from it at this call site).
// Returns `place` unchanged (null passthrough).
template<>
sslSTACK_DATA *New<sslSTACK_DATA>(sslSTACK_DATA *place, const sslSTACK_DATA *val)
{
    if (!place)
        return nullptr;

    place->type = sslSTACK_DATA::TYPE_INT;
    place->int_val = 0;
    *place = *val;
    return place;
}
