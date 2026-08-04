#pragma once
#include "../m3d/m3d_boundary.h"   // _m3dNormalize / _m3dCheckValid / IGNORE_STRONG_ASSERT / STRONG_ASSERT_DUMMY
// Boundary declarations for the aiWATCHER_BRAIN / aiWAH_SYS bodies reversed in the saber2 drain.
// These reach into ai08 objects (aiBRAIN status/mind, unit descriptor) whose full layouts are the
// next frontier — accessed here by DB-verified raw offset only, never a fabricated struct body.

struct aiBRAIN;   // ai08 — per-unit brain (aiBRAIN.h)                          boundary
struct aiSTATUS;  // ai08 — per-brain runtime status block (aiBRAIN::st.pointee) boundary (opaque)

// Shared empty-string / zero-byte global at 0x8200155A, passed by the strong-assert path as its
// trailing "info" byte argument. The reversed bodies reference the IDA raw name byte_8200155A;
// declared here matching the Crash(...,unsigned char msgCtx) overload it resolves to. (2026-07-31)
extern unsigned char empty_string;
extern unsigned char byte_8200155A;

// --- DB-verified raw offsets into ai08 objects touched by these bodies -------------------------
// aiSTATUS: negated reticle-space evade direction (float x/y/z) and the pre-normalisation fallback.
enum {
    aiSTATUS_off_evadeReticleDir = 0x5A8, // preferred evade dir (normalise; fall back if degenerate)
    aiSTATUS_off_evadeFallback   = 0x2C,  // fallback direction used when the reticle dir is null
    aiSTATUS_off_flags           = 0x258, // status flag word; bit 2 (from MSB) suppresses on-path notify
};
// aiBRAIN: mind.best.pointee (aiBEST_BASE*), and the unit descriptor pointer + its enemy caps.
enum {
    aiBRAIN_off_mindBestPointee = 0x1E0, // mind.best.pointee — target of NoticeBeingOnPath forward
    aiBRAIN_off_unitDesc        = 0x54,  // a8 unit descriptor pointer ("desc" in a8_plan_wah)
    aiBRAIN_off_pInst           = 0x15C, // entENTITY::pInst (animINST*) — non-null == instantiated
};
enum {
    aiUNITDESC_off_maxMeleeEnemies  = 0x2F8, // int  — max simultaneous melee attackers
    aiUNITDESC_off_maxRangedEnemies = 0x2FC, // int  — max simultaneous ranged attackers
};

// aiBEST_BASE::NoticeBeingOnPath is virtual slot 0x98 in its vtable; called through a computed
// pointer to avoid asserting the (unreversed) aiBEST_BASE layout.
enum { aiBEST_BASE_vslot_NoticeBeingOnPath = 0x98 };
