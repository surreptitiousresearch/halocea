#pragma once
#include "trkHOLDER.h"
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dMATR.h"
// ws-engine ent: an entity tracker — a resolved reference (holder) to an entity plus an optional
// camera/transform override. DB-verified layout (types_members entTRACKER) — size 0x60. The
// nameTrk dsTSTRING<char> slot stays a byte-accurate opaque span (next frontier).

struct camCAMERA; // cam subsystem — camera   boundary (ptr)
struct m3dV;      // ws m3d — 3-vector        boundary (ptr, GetPos out-param)

struct entTRACKER {
    unsigned char type;            // 0x00 tracker source kind
    unsigned char _pad01[3];       // 0x01 alignment
    trkHOLDER     holder;          // 0x04 resolved entity/instance/object (0x10)
    camCAMERA    *pCamera;         // 0x14
    m3dMATR       matr;            // 0x18 transform override (DB types_members entTRACKER)
    unsigned char nameTrk[4];      // 0x58 dsTSTRING<char> tracker name (opaque)
    bool          usePhysSystem;   // 0x5C
    unsigned char _pad5D[3];       // 0x5D pad to 0x60

    // Resolve the tracked entity's current world-space position into `out`. Body external to this
    // batch (reached by aiBRAIN::implUseSmart* smart-object entry-point selection). boundary.
    void GetPos(m3dV *out) const;

    // 0x82A4C6A8 (??0entTRACKER@@QAA@XZ) — default ctor.  boundary.
    entTRACKER();
    // 0x825348B0 (??1entTRACKER@@QAA@XZ) — dtor.  boundary.
    ~entTRACKER();
    // 0x82994440 (??4entTRACKER@@QAAAAV0@ABV0@@Z) — copy-assign.  boundary.
    entTRACKER &operator=(const entTRACKER &rhs);
    // 0x82A4C848 (?Init@entTRACKER@@QAAHV?$dsTSTRING@D@@@Z) — resolve `name` into a tracker;
    // returns non-zero on success.  boundary.
    int Init(dsTSTRING<char> name);
    // 0x82A4B568 (?GetCenter@entTRACKER@@QBAXPAUm3dV@@@Z) — const: tracked entity's center into `out`.
    // boundary.
    void GetCenter(m3dV *out) const;

    // ?GetMatrix@entTRACKER@@... — write the tracked entity's world-space transform into `out`
    // (`oper` selects the compose mode). Used by aiBRAIN::implTransport.  boundary — body external.
    void GetMatrix(m3dMATR *out, int oper) const;
    // 0x82A4CC70 (?GetName@entTRACKER@@QBAPBDXZ) — const: the tracker's name (may be "").  boundary.
    const char *GetName() const;
};
