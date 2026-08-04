#pragma once
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dV.h"

// ws-engine nav subsystem — a nav waypoint. DB-verified layout (types_members navWP), size 44:
//   flags@0, distReach@4, lenEdgeTooLong@8, penID@12, visCheckAdditionalShift@16,
//   edgeAdditionalShift@28, wpData@40.

struct wpWP;    // nav — waypoint payload   boundary (ptr)
struct m3dMATR; // m3d/m3dMATR.h            boundary (ptr)
struct animINST; // anim instance           boundary (ptr)
struct entENTITY; // ent — parent entity    boundary (ptr)
struct aiBRAIN;  // ai08 — acquiring brain  boundary (ptr)

struct navWP {
    apSTATE_T<unsigned long> flags;      // 0x00 (.state bit31 = disabled)
    float  distReach;                    // 0x04
    float  lenEdgeTooLong;               // 0x08
    short  penID;                        // 0x0C
    m3dV   visCheckAdditionalShift;      // 0x10
    m3dV   edgeAdditionalShift;          // 0x1C
    wpWP  *wpData;                       // 0x28

    // Defined elsewhere (nav).  boundary
    bool  IsEnabled(unsigned int mask);
    float Dist_2(const m3dV *pos);
    // ?Dist@navWP@@QBAMPBUm3dV@@@Z @ 0x831F2EC8 — Euclidean distance to `pos`.  boundary
    float Dist(const m3dV *pos) const;

    // --- debug/query surface used by aiDEBUG::RenderDebugInfoWPs (bodies external — boundary) ---
    void         GetEffectivePos(m3dV *out) const; // effective world position (shifted); DB: QBA (const)
    bool         IsHO() const;                   // this waypoint sits on a high-obstacle edge
    const m3dV  *GetDirToHOPlane() const;        // direction toward the HO plane; DB: QBA (const)
    // 0x831F25D8 (?GetRefObjectVector@navWP@@QBAABUm3dV@@XZ) — the reference (corner) vector from
    // the effective position toward the associated object. boundary — body external.
    const m3dV  &GetRefObjectVector() const;

    // Direction of the waypoint's "up" (snap-plane up) written into `out`. boundary — body external.
    void         GetDirUp(m3dV *out) const;
    // Instance/debug name of this waypoint. boundary — body external.
    const char  *GetName() const;
    // The entity this waypoint is parented to (owning HO object holder). boundary — body external.
    entENTITY   *GetParentObject() const;

    // Recompute the relative HO direction reference after (re)binding.  boundary.
    void         MakeRelativeDirRefHO();
    // Attach this waypoint to model instance `inst` at world matrix `mat`.  boundary.
    void         BindToInst(struct animINST *inst, const m3dMATR *mat);

    // 0x831F4D80 (?IsVisibleThin@navWP@@QBAHPAUm3dV@@PBV1@PAVanimINST@@@Z) — visibility test from
    // `from` to this waypoint (thin capsule), ignoring `ignoreWP`, against instance `inst`. boundary.
    int  IsVisibleThin(m3dV *from, const navWP *ignoreWP, struct animINST *inst) const;
    // 0x831F3188 (?CanAcquireHO@navWP@@QBA_NKK@Z) — whether this HO edge can be acquired given the
    // acquirer id `who` and the enable-mask `mask`. boundary.
    bool CanAcquireHO(unsigned int who, unsigned int mask) const;

    // ?CanAcquireHO@navWP@@QBA_NPAVaiBRAIN@@K@Z — whether `brain` may acquire this HO edge under the
    // blocked-wp mask `mask`. Used by aiBRAIN::implMoveToHO / ForceAFMoveToHO.  boundary.
    bool CanAcquireHO(aiBRAIN *brain, unsigned int mask) const;
    // ?AcquireHO@navWP@@QAAXPAVaiBRAIN@@@Z — mark this HO edge acquired by `brain`.  boundary.
    void AcquireHO(aiBRAIN *brain);
    // ?ReleaseHO@navWP@@QAAXPAVaiBRAIN@@@Z — release this HO edge previously acquired by `brain`. boundary.
    void ReleaseHO(aiBRAIN *brain);
    // 0x831F22E8 (?GetPosVisChk@navWP@@QBAXPBUm3dV@@PAU2@M@Z) — compute the from/to endpoints used
    // for a visibility check toward `ref` (offset by `shift`), writing them into `outFrom`. boundary.
    void GetPosVisChk(const m3dV *ref, m3dV *outFrom, float shift) const;
};
