#pragma once
#include "dsSTRID.h"

struct dsDATA; // ws-engine ds — forward

namespace ds_data {

// Per-T reflection/scripting dispatcher. The dsDATA_TYPE_IMPL<T> virtual overrides
// (GetProperty/SetProperty/IsFunc/IsProperty/CallFunc) forward straight into these statics,
// which resolve named members/methods against T's registered descriptor. Boundary — the
// static bodies are provided by the ds_data reflection runtime, not reversed here.
//
// Single canonical body: ws/ds/dsDATA_TYPE_HELPERS.h and hcex/halo_ds_data_type_boundary.h each
// used to carry a partial copy (IsFunc/IsProperty/CallFunc and Get/SetProperty respectively);
// both now include this file. All five signatures are confirmed against the DB manglings, e.g.
// ?GetProperty@?$dsDATA_TYPE_CLASS@UhaloPLAYER_ACTIONS_ENV@@@ds_data@@SAHVdsSTRID@@ABVdsDATA@@AAV4@@Z
// and ?CallFunc@...@ds_data@@SAHVdsSTRID@@AAVdsDATA@@PAV4@H1@Z (dsSTRID by value, dsDATA by ref).
template<class T>
struct dsDATA_TYPE_CLASS {
    static int GetProperty(dsSTRID id, const dsDATA &inst, dsDATA &val);
    static int SetProperty(dsSTRID id, dsDATA &inst, const dsDATA &val);
    static int IsFunc(const dsDATA &inst, dsSTRID id);
    static int IsProperty(const dsDATA &inst, dsSTRID id);
    static int CallFunc(dsSTRID id, dsDATA &inst, dsDATA *args, int argCount, dsDATA &retval);
};

} // namespace ds_data
