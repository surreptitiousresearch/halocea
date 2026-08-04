#pragma once
// ws-engine ai08 — per-nav-system suspicion cache: the set of attention/"suspicion"
// points a chaser tracks for one navigation system.
// DB-verified layout (types_members nsSUSCACHE) — size 60 (0x3C).

#include "aiSUSPOINT.h"
#include "aiSUSTYPE.h"
#include "../ds/dsTSTRING.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"

struct nsSUSCACHE {
    dsTSTRING<char>            name;             // 0x00 debug name
    short                      nsID;             // 0x04 owning nav-system id
    // 0x06..0x07 padding
    dsVECTOR<aiSUSPOINT, 8>    pts;              // 0x08 tracked suspicion points
    int                        emptyCountdown;   // 0x1C frames until considered empty
    m3dV                       posAssocPlrReal;  // 0x20 real associated-player position
    m3dV                       posAssocPlrSeen;  // 0x2C last-seen associated-player position
    int                        isListenCurFrame; // 0x38 listening this frame flag

    // 0x8320CC10-region ctor: build an empty cache bound to nav-system `nsID`. ai08 boundary.
    nsSUSCACHE(short nsID);
    ~nsSUSCACHE();

    // True when any tracked point ranks better than `type`. ai08 boundary.
    bool HasAnyBetterThan(aiSUSTYPE type);
    // Ensure a random point is present in the cache. ai08 boundary.
    void ConsiderAddingRandom();
    // Copy the current best view point(s) into `wps`; returns false when none. ai08 boundary.
    bool CopyBestView(dsVECTOR<short, 8> *wps, m3dV *outPos);
    // Remove the best entry currently referenced by `wps`. ai08 boundary.
    void DeleteBest(dsVECTOR<short, 8> *wps);
    // Remove any tracked point near `pos`. ai08 boundary.
    void DeleteNear(m3dV *pos);
    // Debug draw the cache's points for suspicion `type` in `color`. ai08 boundary.
    void DebugRender(aiSUSTYPE type, unsigned int color, m3dV *refPos);
    // Update (or insert) the point for suspicion `type` with world position `pos`. ai08 boundary.
    void AssocUpdate(aiSUSTYPE type, m3dV &pos);
};
