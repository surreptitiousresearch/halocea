#pragma once
#include "dsSTRID.h"

struct dsDATA; // ws-engine ds — forward

namespace ds_data {

// Per-T reflection/scripting dispatcher. The dsDATA_TYPE_IMPL<T> virtual overrides
// (GetProperty/SetProperty/IsFunc/IsProperty/CallFunc) forward straight into these statics,
// which resolve named members/methods against T's registered descriptor. Boundary — the
// static bodies are provided by the ds_data reflection runtime, not reversed here.
template<class T>
struct dsDATA_TYPE_CLASS {
    static int GetProperty(dsSTRID id, const dsDATA &inst, dsDATA &val);
    static int SetProperty(dsSTRID id, dsDATA &inst, const dsDATA &val);
    static int IsFunc(const dsDATA &inst, dsSTRID id);
    static int IsProperty(const dsDATA &inst, dsSTRID id);
    static int CallFunc(dsSTRID id, dsDATA &inst, dsDATA *args, int argCount, dsDATA &retval);
};

} // namespace ds_data
