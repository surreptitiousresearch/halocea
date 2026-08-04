#pragma once
#include "dsTSTRING.h"

// ws-engine ds_data: per-T debug-formatting policy. GetDbgString renders a boxed value of type T
// into a human-readable dsTSTRING for debug/inspection surfaces. Types that supply no bespoke
// formatter (the common case, incl. the halo* input env types in this bridge) fall through to the
// primary-template body, which yields the literal "undef". DB-verified: the class carries no data
// members (stateless static policy); GetDbgString is a `static` returning dsTSTRING<char> by value.

namespace ds_data {

template<class T>
struct dsDATA_TYPE_DBG {
    // Format `val` for debug display. Default policy: return the literal "undef".
    static dsTSTRING<char> GetDbgString(const T &val);
};

} // namespace ds_data
