#pragma once
#include "../ds/dsSHARED_PTR.h"
#include "../ds/dsFLAGS.h"
#include "../ds/dsFIXED_BIT_ARRAY.h"
#include "../m3d/m3dV.h"
#include "../ctrl/ctrlPROP_ID.h"   // canonical ctrlPROP_ID
#include "../ctrl/ctrlPROP_LIST.h" // canonical ctrlPROP_LIST
#include "aiSTATUS.h"      // canonical aiSTATUS (gen/body/cs/est/perc/peekerSelf)
#include "aiBEST_BASE.h"   // canonical aiBEST_BASE (HasBHV/GetWannaBePos)
#include "aiWEAPON_BASE.h" // canonical aiWEAPON_BASE (IsShootingNow)
#include "aiPERC_BASE.h"   // canonical aiPERC_BASE (perception module)
#include "aiENEMY_BASE.h"  // canonical aiENEMY_BASE (enemy-tracking module)
#include "aiBODY_IFACE.h"  // canonical aiBODY_IFACE (body interface)

struct m3dMATR;   // m3dMATR.h — 4x3 affine matrix                          boundary (fwd)
struct navSYS;    // nav subsystem — nav-mesh system                        boundary (fwd)

// ctrl subsystem — combine two property ids into a property set (??U@YA?AUctrlPROP_LIST@@ABUctrlPROP_ID@@0@Z
// @ 0x8298D860). boundary — body external to this batch.
ctrlPROP_LIST operator|(const ctrlPROP_ID &a, const ctrlPROP_ID &b);

// ctrl subsystem — hideout position property ids (const ctrlPROP_ID globals). boundary data.
extern const ctrlPROP_ID HO_POS_LEFT;   // ?HO_POS_LEFT@@3UctrlPROP_ID@@B  @ 0x8431AB4C
extern const ctrlPROP_ID HO_POS_RIGHT;  // ?HO_POS_RIGHT@@3UctrlPROP_ID@@B @ 0x8431ABC4
extern const ctrlPROP_ID HO_POS_OUTER;  // ?HO_POS_OUTER@@3UctrlPROP_ID@@B @ 0x8431AB50

// m3d subsystem — global vector/matrix constants read by the else-branches. boundary data.
// Declared `const` to match the canonical m3d header (m3d_boundary.h) and the rest of the codebase.
// (DB mangling is `@@3Um3dV@@A` = non-const in the binary; the source-wide convention treats these
// mathematically-constant basis vectors as const — kept consistent to avoid C2373 across TUs.)
extern const m3dV  m3dVZero;    // ?m3dVZero@@3Um3dV@@A
extern const m3dV  m3dVUnitX;   // ?m3dVUnitX@@3Um3dV@@A
extern const m3dV  m3dVUnitY;   // ?m3dVUnitY@@3Um3dV@@A
extern const m3dV  m3dVUnitZ;   // ?m3dVUnitZ@@3Um3dV@@A
extern const m3dV  m3dVUndef;   // ?m3dVUndef@@3Um3dV@@A  (NaN-filled sentinel vector)
extern m3dMATR     m3dMatrIdentity; // ?m3dMatrIdentity@@3Vm3dMATR@@A

// m3d free helpers (reversed in the m3d subsystem). boundary.
int _m3dCheckValid(const m3dV *v); // debug finiteness assert (side-effect only)
int _m3dNormalize(m3dV *v);        // normalise in place; nonzero when length was non-zero
// 0x83309AC8 (?m3dIsBelongPointDblCone@@YAHPBUm3dV@@00MMMM@Z) — true when `point` lies inside the
// double sight cone rooted at `apex` along `dir` (straight/side radii + straight/side half-angles).
int m3dIsBelongPointDblCone(const m3dV *point, const m3dV *apex, const m3dV *dir,
                            float radStraight, float radSide,
                            float angleStraight, float angleSide);
// Boundary interface subsets for the aiWATCHER_BRAIN accessor batch (0x8316FA80..0x831700E8).
// These declare ONLY the members/methods those accessors reach through the tracked aiBRAIN's
// mind/body/status modules — they are the next re-source frontier, so their bodies live
// elsewhere. Data-member offsets carry a DB (types_members) reference; padded subsets reproduce
// the touched offsets exactly. Do not treat the padded structs as full authoritative layouts.

struct entENTITY;   // entENTITY.h — root game entity (grenade / enemy args)   boundary (fwd)
struct aiBRAIN;     // ai08 — per-unit AI brain                                boundary (fwd)
struct dsSTRID_FLAGS; // dsSTRID_FLAGS.h

// mdl subsystem — an "interactive transform reference component" (hideout/model transform snap).
// Canonical layout + DB-verified 33-slot vtable live in mdl/mdlITRC_BASE.h (DB size 8:
// __vftable@0, stateUser@4). The brain-watcher matrix accessors reach IsHO/IsValid/GetSnapMatr/
// GetBaseMatr, all provided as inline wrappers there.
#include "../mdl/mdlITRC_BASE.h"

// Debug/desc access: the brain descriptor's strike-immune damage-type set. The binary reads it as
// a dsSTRID_FLAGS embedded in the (derived) descriptor reached through the brain's iaIACTOR spDesc
// — modelled as a boundary accessor (the desc layout is reversed elsewhere). boundary.
const dsSTRID_FLAGS *aiBrainStrikeImmuneFlags(const aiBRAIN *brain);

// ai08 — behaviour ids the "best behaviour" module can currently be running (DB enum aiBID).
enum aiBID {
    AI_BID_UNDEF            = -1,
    AI_BID_SIMPLE          = 0,
    AI_BID_DEAD            = 1,
    AI_BID_CINE            = 2,
    AI_BID_ROAM            = 3,
    AI_BID_PATH            = 4,
    AI_BID_MAJOR_PATH      = 5,
    AI_BID_HO_PATH         = 6,
    AI_BID_HO_SPEC_MOVE    = 7,
    AI_BID_AVOID           = 8,
    AI_BID_CHASE           = 9,
    AI_BID_SNS             = 10,
    AI_BID_DNS             = 11,
    AI_BID_HO              = 12,
    AI_BID_MAJOR_HO        = 13,
    AI_BID_STAY            = 14,
    AI_BID_SNIPE           = 15,
    AI_BID_SHOOT           = 16,
    AI_BID_MELEE           = 17,
    AI_BID_TURRET          = 18,
    AI_BID_IDLE            = 19,
    AI_BID_IDLE_SMART      = 20,
    AI_BID_HLB             = 21,
    AI_BID_SMART           = 22,
    AI_BID_MAJOR_SMART     = 23,
    AI_BID_BALCONY         = 24,
    AI_BID_EMPTY_HANDS     = 25,
    AI_BID_CHASE_GO_LAST_VIEW = 26,
    AI_BID_SNS_LOST        = 27,
    AI_BID_LAST            = 28,
    AI_BID_GVT_MAJOR_PATH  = 29,
    AI_BID_ZERO_PATH       = 30,
    AI_BID_ZERO_SHOOT      = 31,
    AI_BID_ZERO_HO         = 32,
    AI_BID_TEACH_HO        = 33,
};

// aiPERC_BASE (mind.perc) is defined once in aiPERC_BASE.h (included above); its NoticeGrenade/
// NoticeGrenadeTerm/SetPar/ProcessInit/NoticeWounded surface is the canonical superset.

// aiBEST_BASE (mind.best) is defined once in aiBEST_BASE.h (included above). HasBHV(int) accepts
// aiBID enumerators via the implicit enum->int conversion.

struct aiWATCHER; // aiWATCHER.h — base watcher   boundary (fwd)

// aiENEMY_BASE (mind.enemy) is defined once in aiENEMY_BASE.h (included above); its Is(const
// entENTITY*)/GetPeeker surface is the canonical superset.

// aiWEAPON_BASE (body.wpn) is defined once in aiWEAPON_BASE.h (included above).

// aiBODY_IFACE (aiBRAIN.body) is defined once in aiBODY_IFACE.h (included above). Its
// GetRefPropCache (const ctrlPROP_LIST*), GetCurrentItrcHO and GetCurrentItrc surface — plus the
// full DB-verified data layout (wpn@0x10, hoSys@0x18) — is the canonical superset of the subset that
// previously lived here.

// aiSTATUS (aiBRAIN.st) is defined once in aiSTATUS.h (included above). The accessors reach its
// gen/body flags, cs.posBody/.posFace/.dirFace, est flags, perc.grAvoidRadius/.needToReduceVision
// and peekerSelf/dbgDuplicatePeekerSelf — all now typed at their DB offsets in the canonical header.
