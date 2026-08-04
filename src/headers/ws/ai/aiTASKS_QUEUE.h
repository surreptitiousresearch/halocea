#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/REF_TYPE.h"
// ws-engine ai08: the mind's queued-task storage (aiMIND::qot). DB-verified layout
// (types_members aiTASKS_QUEUE) — size 24 (0x18): __vftable@0,
// storage@4 (dsVECTOR<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>,8>, 20).
// Methods/vtable are the next frontier; only the layout is modelled (explicit __vftable member,
// no virtual methods — cdtREFINE precedent).

struct aiTASKS_QUEUE_vtbl;   // vtable  boundary (fwd)
struct dsVECTOR_PARAM_LIST;  // ds — parameter-list payload of a queued task  boundary (fwd)

struct aiTASKS_QUEUE {
    aiTASKS_QUEUE_vtbl *__vftable; // 0x00
    dsVECTOR<ds_data::REF_TYPE<dsVECTOR_PARAM_LIST>, 8> storage; // 0x04 queued tasks (FIFO)
};
