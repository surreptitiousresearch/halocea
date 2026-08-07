#pragma once
// Misc externs HALO_SOUND_SYSTEM's methods touch that don't belong to any single sibling type:
// os-layer helpers, the Blam tag-name lookup, dbg-console tunables, and the fixed FSB
// sound-list table. None of these are reversed here (boundary / other-subsystem globals);
// declared only far enough to type HALO_SOUND_SYSTEM's call sites.

#include <stdint.h>
#include "../ws/ds/dsTSTRING.h"

// ---- os-layer (real bodies already exist in src/ws/os/, declared via os_boundary.h normally;
// redeclared here narrowly since those .cpp files don't expose a shared prototype header) ----
extern "C" {
    unsigned int osGetCurThreadId();                  // src/ws/os/osGetCurThreadId.cpp
    unsigned int osGetTime();                          // src/ws/os/osGetTime.cpp
    void         osOutputDebugString(const char *fmt, ...); // boundary — printf-style wrapper over _osOutputDebugString
    char        *tag_get_name(short tag_index);         // src/tag_get_name.c
}

// dbg-console tunables with change-detection (Update() compares .value against .prevValue to
// notice a live edit). boundary — the ws-engine dbgVAR subsystem.
//
// DEVIATION (odr_dup drain): these were declared as a locally-defined `template<class T> struct
// DBG_VAR { T value; T prevValue; };` — a body this header shared, verbatim, with
// HALO_SOUND_LIST_HALO_CHANNEL_globals.h and ws/ui_new/ui_stat_boundary.h (three file-scope
// definitions of one name). It was also the WRONG LAYOUT: `applied_types` spells every one of these
// symbols `dbgVAR_SIMPLE<T,N>`, whose members (types_members dbgVAR_IMPL<T,N>) are a 12-byte dbgVAR
// base (__vftable@0, name@4, typeId@8) then value@0x0C, prevValue, defaultValue. DBG_VAR put
// `.value` at offset 0, i.e. on the high byte of the vtable pointer. The read at 0x836C30C0 in
// HALO_CHANNEL::CheckNotReady is `lbz r8, dbgVAR_SIMPLE<bool,1>.value(r10)` — offset 0x0C.
// Replaced by the canonical, DB-verified templates; `.value` / `.prevValue` are inherited, so no
// consumer expression changes.
#include "../ws/wb/dbgVAR_boundary.h"   // dbgVAR / dbgVAR_IMPL<T,N> / dbgVAR_SIMPLE<T,N>

extern "C" dbgVAR_SIMPLE<bool, 1> dbg_disableSounds_0;  // globally mutes HALO_SOUND_SYSTEM::FindSoundByName
extern "C" dbgVAR_SIMPLE<int, 2>  dbg_mouthMinDelay;    // -1 == "use HALO_SOUND_PARAMS::mouthMinDelay"
extern "C" dbgVAR_SIMPLE<bool, 1> dbg_pauseHaloSounds;  // dbg-console pause toggle, polled from Update()
extern "C" int            maxPlaying;               // high-water mark logged by PrintUsedSounds
extern bool            hcex_mouth_skull;         // HCEX lip-sync/skull-VO feature toggle

// One entry of the fixed 15-element FSB sound-list table (types_members
// _EC05441F58351F3B07D4C7AF18902550): prepareStreams@0, name@4, filename@8, level@12 — size 16.
// Anonymous aggregate-literal type in the original source; DB $-name kept as the tag
// ($ in identifiers is an MSVC extension), aliased to a readable name.
typedef struct _EC05441F58351F3B07D4C7AF18902550 {
    int          prepareStreams; // 0x00
    const char  *name;            // 0x04
    const char  *filename;         // 0x08
    const char  *level;             // 0x0C
} _EC05441F58351F3B07D4C7AF18902550;
typedef _EC05441F58351F3B07D4C7AF18902550 SOUND_LIST_ENTRY;
extern "C" SOUND_LIST_ENTRY SOUND_LIST_LST[15]; // fixed table, one entry per HALO_SOUND_SYSTEM::soundLists slot

// Blam sound-manager config globals (`sound_manager_globals_t sound_manager_globals;`).
// This header used to carry its own copy of the 188-byte body. Both copies agreed with the DB
// (types_members sound_manager_globals_t) member-for-member — initialized@0 … sound_variety@0xB8,
// listeners@0x18 as sound_listener[2] (DB size 136) — and differed only in how the two padding gaps
// were spelled (`_pad15[3]`/`_padB7` here vs `_pad0[3]`/`_pad1[1]` there). The basename-matching
// header wins; it also carries the real platform_sound_manager_definition rather than the opaque
// forward declaration this copy used.
#include "../sound_manager_globals_t.h"
extern "C" sound_manager_globals_t sound_manager_globals;
