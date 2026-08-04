#pragma once
#include "SYSTEM.h"
#include "BUFFER.h"
#include "BUFFER_PARAMS.h"
#include "../ds/LIST.h"
#include "../ds/dsSTRID.h"
// ws-engine snd: the FMOD Ex backend of the sound-driver singleton. Derives from snd::SYSTEM.
// DB-verified layout (types_members snd::SYSTEM_FMOD) — size 2320 (0x910). Only the members the
// reversed GetBuffer path touches are spelled out; the large FMOD/vector/map interior regions
// are preserved as exact-size opaque fillers (their full types are owned by the src/ws/snd
// drain), keeping every named member at its DB offset.

namespace snd {

// Concrete FMOD-backed buffer; layout owned by the src/ws/snd drain. Derives from snd::BUFFER
// (base@0, 32B) — GetBuffer only ever handles it through snd::BUFFER* and the two calls below.
struct BUFFER_FMOD; // boundary

// Comparator functor passed to implFind: matches a stored buffer by its event name.
// DB-verified layout (types_members snd::CMP_EQ): name@0 (dsSTRID) — size 4.
typedef struct CMP_EQ {
    dsSTRID name; // 0x00 event name to match
} CMP_EQ;

// Debug/diagnostics sub-object (types_members snd::SYSTEM_DBG_FMOD, size 1244; DB re-checked
// 2026-08-04 — 55 member rows exist, interior dominated by ds::MAP/osTIMER FMOD state). Kept as
// an exact-size opaque blob per the FMOD-boundary adjudication (rc2026 OPAQUE_BRIEF §3): the
// full layout is owned by the src/ws/snd drain; only the members GetBuffer reads are exposed.
typedef struct SYSTEM_DBG_FMOD {
    unsigned char _opaque[1244]; // 0x000..0x4DC full interior (boundary)

    // boundary — dbg-console per-sound filters (bodies in the src/ws/snd drain).
    bool IsBreakOn(const char *path);
    bool IsSkipped(const char *path);

    // Print-play-spam gate: a bool inside the `playStats` map wrapper at struct offset 1004
    // (DB: playStats@1000, the enable flag is the byte at +4). When set, GetBuffer applies the
    // break-on-play filter on both the success and failure paths.
    bool PrintPlayEnabled() const { return _opaque[1004] != 0; }
} SYSTEM_DBG_FMOD;

struct SYSTEM_FMOD : SYSTEM {
    unsigned char _fmodHead[84];             // 0x034..0x088 system/master/reverb/projects/lstPos/
                                             //             nativeCategories/events (boundary)
    ds::LIST<BUFFER_FMOD *> buffers;         // 0x088 live playing buffers
    unsigned char _counters[16];             // 0x09C mostAudible/numPlaying/numStreamPlaying/
                                             //       getBufferCounter (boundary)
    ds::LIST<BUFFER_FMOD *> virtualEvents;   // 0x0AC buffers that started virtual (silent)
    unsigned char _mid[880];                 // 0x0C0..0x430 streamedLowPriority/maps/timers/
                                             //             resources/reverb state (boundary)
    SYSTEM_DBG_FMOD          dbg;            // 0x430 (1244B)
    dsTSTRING<char>          forcePreset;    // 0x90C

    // 0x82E36928 — resolve the named event and, if found and audible, allocate + init a
    // BUFFER_FMOD for it, register it (and any virtual instance), and return it as snd::BUFFER*.
    // Returns nullptr when the system is down, called off-thread, FMOD memory is exhausted, the
    // sound is filtered/skipped, the name is missing from the .fev, or init fails.
    BUFFER *GetBuffer(dsSTRID name, const BUFFER_PARAMS &params);

    // 0x82E38CB8 (?SetPreset@SYSTEM_FMOD@snd@@UAAXV?$dsTSTRING@D@@@Z) — snd::SYSTEM backend
    // dispatch slot 0x44. Apply the named FMOD reverb preset (asserts owning-thread, forwards to
    // SetPresetForced when the name differs from forcePreset). Arg is by value in the mangle
    // (V?$dsTSTRING@D@@); the PPC ABI lowers it to a hidden pointer (r4) — see the decompile which
    // reads `dsTSTRING<char> *preset`. Body owned by the src/ws/snd drain; declared here so the
    // boundary shim can cast SYSTEM* -> SYSTEM_FMOD* and call it. // boundary
    void SetPreset(dsTSTRING<char> preset);

    // boundary — linear search of `buffers` for an entry matching `cmp`; returns index or -1.
    // Template body (per CMP instantiation) lives in the src/ws/snd drain.
    template<class CMP> int implFind(const CMP *cmp);
};

} // namespace snd
