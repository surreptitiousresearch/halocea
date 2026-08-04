#pragma once
#include "../ds/REF_TYPE.h"
#include "../ds/dsEVENT_HANDLER.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/LIST.h"
// ws-engine gs: per-user config manager. Owns a default apCONFIG handle and a task list; hands
// out a user's apCONFIG handle via Get(userIdx). DB-verified layout (types_members
// gsUSER_CFG_MNG, 40 bytes): __vftable@0, eventDispatcher@4 (8B), defaultCfg@12
// (ds_data::REF_TYPE<apCONFIG>), initialized@16 (bool), tasks@20 (ds::LIST<TASK_INFO>).
// Nested EVENT_DISP / TASK_INFO bodies reconciled against types_members 2026-08-04.

struct apCONFIG;
struct dsREF_COUNT;         // ds intrusive refcount base (policy parameter only)
struct gsUSER_CFG_MNG_vtbl; // boundary — gs subsystem vtable
namespace gts { struct TASK; } // gts task (pointer only, via smart pointer)

typedef struct gsUSER_CFG_MNG {
    // gsUSER_CFG_MNG::TASK_INFO — DB-verified: userID@0, task@4 — size 8.
    struct TASK_INFO {
        int userID;                                                          // 0x00
        dsSMART_PTR<gts::TASK, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > task; // 0x04
    };

    gsUSER_CFG_MNG_vtbl                *__vftable;       // 0x00
    // gsUSER_CFG_MNG::EVENT_DISP_gsUSER_CFG_MNG — DB-verified: dsEVENT_HANDLER base@0, pObj@4.
    struct EVENT_DISP_gsUSER_CFG_MNG : dsEVENT_HANDLER {
        gsUSER_CFG_MNG *pObj; // 0x04 back-pointer to the owning manager
    } eventDispatcher;                                   // 0x04 (8B)
    ds_data::REF_TYPE<apCONFIG>        defaultCfg;       // 0x0C
    bool                               initialized;      // 0x10
    unsigned char                      _pad11[3];        // 0x11 db-verified padding
    ds::LIST<TASK_INFO>                tasks;            // 0x14 (20B) to size 40

    // 0x823D29E0 — return the primary user's apCONFIG handle: forwards to Get(userIdx) with the
    // user manager's primaryUserIdx. Reversed in gsUSER_CFG_MNG__Get.cpp. Returns REF_TYPE by value.
    ds_data::REF_TYPE<apCONFIG> Get();

    // ?Get@gsUSER_CFG_MNG@@QAA?AV?$REF_TYPE@VapCONFIG@@@ds_data@@H@Z — the config handle for the
    // given user index, returned by value (sret). boundary (outside this batch).
    ds_data::REF_TYPE<apCONFIG> Get(int userIdx);
} gsUSER_CFG_MNG;

// ws-engine gs: user manager singleton — modeled here only up to primaryUserIdx@0x20 (the one
// field the config-manager wrapper reads). types_members gsUSER_MNG. boundary.
typedef struct gsUSER_MNG {
    unsigned char _pad00[0x20]; // 0x00 — engine-internal user table (not modeled here)
    int           primaryUserIdx; // 0x20
} gsUSER_MNG;

extern gsUSER_MNG *gsUserMng;
