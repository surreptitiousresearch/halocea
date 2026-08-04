#pragma once
#include "../ent/entENTITY.h"
#include "../DEFAULT_CTOR.h"
#include "../ds/dsTSTRING.h"
#include "../m3d/m3dV.h"
// ws-engine ai08: the public interface subobject of an aiBRAIN — an entENTITY plus a lazily
// resolved game-info pointer. DB-verified layout (types_members aiBRAIN_IFACE) — size 0x1C0:
//   entENTITY@0 (0x1BC, base), __propGameInfo@0x1BC (DEFAULT_CTOR<propGAME_INFO*>).

struct propGAME_INFO;      // prop subsystem — per-actor game info               boundary (ptr)
struct propGAME_INFO_TEAM; // propGAME_INFO_TEAM.h

struct aiBRAIN_IFACE : entENTITY {
    DEFAULT_CTOR<propGAME_INFO *> __propGameInfo; // 0x1BC

    // Virtual — return the brain's team (points at the cached propGAME_INFO_TEAM). Used via
    // ->team.id by aiPLANNER::AlertTeam.  boundary (body external to this batch).
    // DB mangle ?GetTeamID@aiBRAIN_IFACE@@QBAABVpropGAME_INFO_TEAM@@XZ is const (QBA) — declared
    // const so const-qualified callers (aiCHEAP_VISION) can reach it.
    propGAME_INFO_TEAM *GetTeamID() const;

    // ?propGameInfo@aiBRAIN_IFACE@@QAAPAVpropGAME_INFO@@XZ @0x83193940 — resolve (lazily) the
    // actor's game-info block; null when the actor has none.
    // REVERSED: src/ws/ai/aiBRAIN_IFACE__propGameInfo.cpp.
    propGAME_INFO *propGameInfo();
    // ?propGameInfo@aiBRAIN_IFACE@@QBAPBVpropGAME_INFO@@XZ @0x83193A88 — const overload (identical
    // body). REVERSED: src/ws/ai/aiBRAIN_IFACE__propGameInfo_const.cpp.
    const propGAME_INFO *propGameInfo() const;

    // ?LocalHeightAndDistXZ@aiBRAIN_IFACE@@UBAXABUm3dV@@0AAM1@Z @0x83193800 — virtual const:
    // decompose the `from`->`to` offset into its up (height) and flat-XZ components and report each
    // component's length. REVERSED: src/ws/ai/aiBRAIN_IFACE__LocalHeightAndDistXZ.cpp.
    virtual void LocalHeightAndDistXZ(const m3dV &from, const m3dV &to,
                                      float &heightDiff, float &distXZ) const;
    // (DecompToUpAndFlatXZ — the virtual this dispatches — is declared below.)

    // Virtual (sret) — copy the brain's display name into `out`. Used by
    // aiEXIFACE::GetDbgWatcheeName.  boundary.
    void GetName(dsTSTRING<char> *out) const;

    // Virtual — number of behaviors this brain currently owns. Inferred vtbl-dispatch slot from
    // the typed msg-iter loop in aiPLANNER::GetTotalBehaviorsCount.  boundary (body external).
    int GetBehaviorsCount();

    // Virtual — this brain's on-screen debug index (-1 when not displayed). Inferred vtbl-dispatch
    // slot from aiPLANNER::FindWithOnScreenIdx.  boundary (body external).
    int GetOnScreenIdx() const;

    // -- geometry accessors (virtual, sret m3dV). Bodies external to this batch.  boundary --
    virtual m3dV GetPos() const;      // world position of the actor
    virtual m3dV GetDir() const;      // forward direction
    virtual m3dV GetDirUp() const;    // up direction
    virtual m3dV GetDirRight() const; // right direction

    // 0x831934B8 (?Dist@aiBRAIN_IFACE@@QBAMPBUm3dV@@@Z) — distance from this actor's position to
    // `posFrom` (3-D). REVERSED: aiBRAIN_IFACE__Dist.cpp
    float Dist(const m3dV *posFrom) const;

    // 0x83193508 (?Dist_2@aiBRAIN_IFACE@@QBAMPBUm3dV@@@Z) — alternate distance metric (m3dDist_2)
    // from this actor's position to `posFrom`. REVERSED: aiBRAIN_IFACE__Dist_2.cpp
    float Dist_2(const m3dV *posFrom) const;

    // 0x83193558 (?GetBodyPosNav@aiBRAIN_IFACE@@UBA?AUm3dV@@XZ) — virtual, sret. The nav-space body
    // reference position: body position plus 0.75 * body up-direction.
    // REVERSED: aiBRAIN_IFACE__GetBodyPosNav.cpp
    virtual m3dV GetBodyPosNav() const;

    // 0x83193630 (?GetBodyMatrNav@aiBRAIN_IFACE@@UBAXAAVm3dMATR@@@Z) — virtual. Build the nav-space
    // body local-coordinate matrix (origin = GetBodyPosNav, axes = normalized right/up/forward).
    // REVERSED: aiBRAIN_IFACE__GetBodyMatrNav.cpp
    virtual void GetBodyMatrNav(m3dMATR &matr) const;

    // 0x831936F8 (?DecompToUpAndFlatXZ@aiBRAIN_IFACE@@UBAXABUm3dV@@AAU2@1@Z) — virtual. Decompose
    // `src` into its projection `up` onto the actor's up-direction and the remainder `flatXZ`.
    // REVERSED: aiBRAIN_IFACE__DecompToUpAndFlatXZ.cpp
    virtual void DecompToUpAndFlatXZ(const m3dV &src, m3dV &up, m3dV &flatXZ) const;
};
