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

// System state bits; only bit 0 (live/initialised) is exercised by the reversed methods.
enum SYSTEM_STATE {
    SST_INITED = 1, // system is up and driving buffers
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

// ?THREAD_ID@snd@@3IA — owning-thread id every public snd method asserts against.
extern unsigned int THREAD_ID;

} // namespace snd
