#pragma once
// ws-engine nav subsystem — the payload behind a navWP (navWP::wpData). The waypoint's geometry and
// HO child bookkeeping live here. DB layout is the next frontier; only the transform/child query
// methods reached by the ai08 move/teleport commands are declared. boundary — bodies external.

#include "../ap/apSTATE_T.h" // apSTATE_T<unsigned long> for ExtendToTRK

struct m3dV;      // ws m3d — 3-vector    boundary (ptr)
struct m3dMATR;   // ws m3d — 4x4 matrix  boundary (ptr)
struct entENTITY; // entENTITY.h  boundary (ptr)

struct wpWP {
    // Promote this payload to a TRK (tracked) waypoint governed by `flags`; returns the
    // (possibly reallocated) payload pointer.  boundary. (Was a duplicate def in aiDEBUG_boundaries.h.)
    wpWP *ExtendToTRK(apSTATE_T<unsigned long> &flags);
    // Set the game object this waypoint is parented to.  boundary.
    void  SetParentObject(entENTITY *obj);
    // ?TransformPoint@wpWP@@... — map local point `p` through this waypoint's transform into `out`.
    // Used by aiBRAIN::ForceAFMoveToNavSys / ForceAFMoveToHO (with the zero vector -> waypoint origin).
    void TransformPoint(const m3dV *p, m3dV *out) const;
    // ?GetMatrix@wpWP@@... — write this waypoint's world-space transform into `out`. Used by
    // aiBRAIN::implTransport.
    void GetMatrix(m3dMATR *out) const;
    // ?HaveChilds@wpWP@@... — true when this HO waypoint already has acquired child hand-offs (so it
    // must not be re-acquired). Used by aiBRAIN::implMoveToHO / ForceAFMoveToHO.
    bool HaveChilds() const;
};
