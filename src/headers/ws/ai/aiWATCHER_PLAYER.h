#pragma once
#include "aiWATCHER.h"
#include "../ds/WEAK_PTR.h"
#include "../m3d/m3dV.h"

// ws-engine ai08 — a watcher bound to a player entity. DB-verified layout (types_members
// aiWATCHER_PLAYER): aiWATCHER base @0x00 (0x4C), player@0x4C, cached body/face/reticle vectors,
// isDead@0xA4, useProcessor@0xA8, wpnCtrl@0xAC, pFireable@0xB0, bodyFsm@0xB4. Total size 0xB8.

struct entENTITY;      // entENTITY.h — tracked player entity          boundary (fwd)
struct usePROCESSOR;   // use subsystem — hideout/ITRC processor        boundary (fwd)
struct wpnLIST_BASE;   // wpn subsystem — weapon list                   boundary (fwd)
struct propFIREABLE;   // prop subsystem — fireable/health component     boundary (fwd)
struct propFSM;        // prop subsystem — finite state machine          boundary (fwd)
struct m3dMATR;        // m3dMATR.h — 4x4 matrix                         boundary (fwd)

struct aiWATCHER_PLAYER : aiWATCHER {
    ds::WEAK_PTR<entENTITY> player;      // 0x4C weak ref to the player entity
    m3dV          cachedBodyPos;         // 0x50
    m3dV          cachedBodyDir;         // 0x5C
    m3dV          cachedBodyUp;          // 0x68
    m3dV          cachedFacePos;         // 0x74
    m3dV          cachedFaceDir;         // 0x80
    m3dV          cachedReticleDir;      // 0x8C
    m3dV          cachedReticleStart;    // 0x98
    bool          isDead;                // 0xA4
    usePROCESSOR *useProcessor;          // 0xA8 hideout/ITRC transform processor
    wpnLIST_BASE *wpnCtrl;               // 0xAC
    propFIREABLE *pFireable;             // 0xB0 health/fireable component
    propFSM      *bodyFsm;               // 0xB4

    // Virtuals / helpers reached by this batch. Bodies are ai08 (some reversed in src/ws/ai).
    bool IsWlnMode() const;              // wall-lean mode gate (virtual)               boundary
    // 0x83290610 (?Update@aiWATCHER_PLAYER@@UAA_NM@Z) — virtual per-frame tick: refresh the body
    // coordinate system + STRID flags, then run the base tick.
    // REVERSED: src/ws/ai/aiWATCHER_PLAYER__Update.cpp
    bool Update(float dt);
    // 0x83290698 (?GetBodyPosNav@aiWATCHER_PLAYER@@UBA?AUm3dV@@XZ) — virtual const, sret: nav-space
    // body reference position. REVERSED: src/ws/ai/aiWATCHER_PLAYER__GetBodyPosNav.cpp
    m3dV *GetBodyPosNav(m3dV *out) const;
    // 0x83290A50 (?GetHOMatrSnap@aiWATCHER_PLAYER@@UAA_NAAVm3dMATR@@@Z) — virtual: write the current
    // hideout snap matrix into `mSnap`; true when a live hideout ITRC was found (else identity).
    // REVERSED: src/ws/ai/aiWATCHER_PLAYER__GetHOMatrSnap.cpp
    bool GetHOMatrSnap(m3dMATR &mSnap);
    // 0x8328FA58 (?UpdateBodyCS@aiWATCHER_PLAYER@@IAAXXZ) — refresh the cached player body
    // coordinate system (position/dir/up), face point, and player-camera reticle ray for the frame.
    // REVERSED: src/ws/ai/aiWATCHER_PLAYER__UpdateBodyCS.cpp
    void UpdateBodyCS();

    // 0x8328DC58 (?GetBodyCS@aiWATCHER_PLAYER@@UBAXAAUm3dV@@00@Z) — virtual const: forward the
    // player's cached body coordinate system (position + forward + up).
    // REVERSED: src/ws/ai/aiWATCHER_PLAYER__GetBodyCS.cpp
    void GetBodyCS(m3dV &pos, m3dV &dir, m3dV &dirUp) const;
    // 0x8328DD40 (?GetPreferredEvadeDir@aiWATCHER_PLAYER@@UBA?AUm3dV@@ABU2@@Z) — virtual const:
    // negated cached body forward, replaced by (bodyUp x bodyDir) when that is too parallel to the
    // up-flattened offset toward `posEvader`. REVERSED: src/ws/ai/aiWATCHER_PLAYER__GetPreferredEvadeDir.cpp
    m3dV GetPreferredEvadeDir(const m3dV &posEvader) const;
    // 0x8328E750 (?IsInLowHO@aiWATCHER_PLAYER@@UBA_NXZ) — virtual const: in the low hideout slot.
    // REVERSED: src/ws/ai/aiWATCHER_PLAYER__IsInLowHO.cpp
    bool IsInLowHO() const;

    // Reversed in this batch (src/ws/ai/aiWATCHER_PLAYER__*.cpp).
    bool IsHOSideLeft() const;                    // 0x8328E800 (virtual)
    bool IsHOSideRight() const;                   // 0x8328E8A0 (virtual)
    m3dV GetHideoutDir() const;                   // 0x8328E938 (virtual)
    bool IsDead() const;                          // 0x8328EB58 (virtual)
    void GetBodyMatrNav(m3dMATR &matr) const;     // 0x8328EC60 (virtual)
    unsigned int GetUID() const;                  // 0x8328ED48 (virtual)
};
