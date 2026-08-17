#pragma once
#include "../ds/dsFLAGS.h"
#include "../ds/dsVECTOR.h"
#include "../ds/LIST.h"
#include "BUFFER.h"
// ws-engine snd: the low-level sound-driver singleton base class
// (D:\Projects\code\common\src.sys\drv\sound\snd.cpp). Concrete backends (snd::SYSTEM_FMOD)
// derive from this. DB-verified layout (types_members snd::SYSTEM): __vftable@0, state@4
// (dsFLAGS<SYSTEM_STATE,int>), categories@8 (dsVECTOR<CATEGORY,8>, 20B), needToUpdate@28
// (ds::LIST<BUFFER*>), listenersCount@48 (int) — size 52.

namespace snd {

// System state bits. Full DB enumerator set (types_enum_values snd::SYSTEM_STATE, ordinal 1237,
// 6 values) — verbatim, not inferred. Only bit 0 is exercised by the reversed methods so far.
enum SYSTEM_STATE {
    SST_INITED            = 1,  // system is up and driving buffers
    SST_EAX               = 2,  // backend reports EAX/reverb capability
    SST_EAX_ENABLED       = 4,  // EAX/reverb processing currently enabled
    SST_MUTED             = 8,  // all output muted
    SST_UPDATE_CATEGORIES = 16, // category volumes dirty; re-push on next update
    SST_USER              = 32, // first backend-defined bit
};

struct SYSTEM_vtbl; // boundary — full backend dispatch slot table owned by the src/ws/snd drain
struct CATEGORY;    // boundary — dsVECTOR element, pointer/size only

typedef struct SYSTEM {
    SYSTEM_vtbl                 *__vftable;     // 0x00
    dsFLAGS<SYSTEM_STATE, int>   state;         // 0x04
    dsVECTOR<CATEGORY, 8>        categories;    // 0x08 (20B)
    ds::LIST<BUFFER *>           needToUpdate;  // 0x1C buffers awaiting a delayed start / update
    int                          listenersCount;// 0x30
} SYSTEM;

// ?System@snd@@3PAVSYSTEM@@A — the process-wide sound-driver singleton.
extern SYSTEM *System;

// ?THREAD_ID@snd@@3HA — owning-thread id every public snd method asserts against.
// DEVIATION: was declared unsigned citing a @@3IA mangling that does not exist in the DB;
// the sole symbol is ?THREAD_ID@snd@@3HA @0x84315E18 (int), and every compare is signed cmpw.
extern int THREAD_ID;

} // namespace snd
