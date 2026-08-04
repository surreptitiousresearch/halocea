#pragma once
#include "m3dV.h"
// ws-engine m3d math: axis-aligned bounding box.
// DB-verified layout (types_members m3dBOX): bll@0 (m3dV), fur@12 (m3dV) — size 24.
// bll = back-lower-left (min corner), fur = front-upper-right (max corner).

struct m3dOBB;
struct m3dMATR;

typedef struct m3dBOX {
    m3dV bll;
    m3dV fur;

    // 0x8265C810
    void CalcStart();
    // 0x8265C840 — grow to contain a point.
    void CalcUpdate(const m3dV *v);
    // 0x8265C9A8 — grow to contain another box (overload __2).
    void CalcUpdate(const m3dBOX *pB);
    // 0x8265BEB8
    void Expand(float value);
    // 0x8265BF08
    int Check() const;
    // 0x8265FD80 — fit the box to an array of points, enforcing a minimum side length.
    // Mangle PAUm3dV: the point array is a non-const m3dV*.
    void Calc(m3dV *pointList, int nPoint, float sideMinLen);
    // 0x8265E830 — represent as an oriented box with identity axes.
    void ConvToOBB(m3dOBB *obb) const;
    // 0x8265EC58 — build the box spanning two points.
    void MakePointPoint(m3dV *p1, m3dV *p2);
    // 0x8265E690 — transform into an oriented box.
    void Transform(m3dMATR *pLCS2WCS, m3dOBB *obb);
    // 0x8265FFF0 — transform and re-fit an axis-aligned box (overload __2).
    void Transform(m3dMATR *pLCS2WCS, m3dBOX *bb);

    // 0x82A71128 — transform into an oriented box using the "advanced" (already-scaled) matrix
    // path used by objOBJ::GetOBB. boundary — not reversed in this batch.
    void Transform2OBB_ADV(m3dMATR *pLCS2WCS, m3dOBB *obb); // boundary

    // Fill `outVertices[8]` with the box's eight corner points. boundary — used by
    // scrSCORCH_MANAGER::FindAround.
    void GetVertices(m3dV *outVertices) const; // boundary

    // 0x8265BDB0 — is `p` inside (or on) the box on all three axes? const. boundary.
    int IsBelongPoint(const m3dV *p) const; // boundary

    // 0x8265C008 — distance test of `point` against the box, early-out beyond `distMax`.
    // Returns 2 when the point is inside, 1 when outside but within `distMax`, 0 otherwise.
    // When non-null: `vClosest` gets the closest point, `dist` the distance, `faceNmb` the face
    // index (1..5 per crossed face). REVERSED: src/ws/m3d/m3dBOX__DistPoint.cpp.
    // DEVIATION: mangled name (PBUm3dV MPAU2 PAM PAH) + prologue (args land in r6/r7/r8 because
    // the float distMax consumes the r5 GPR slot) prove the signature is
    // (const m3dV*,float,m3dV*,float*,int*), not the earlier (m4dV*,float,m3dV*,void*,float*).
    int DistPoint(const m3dV *point, float distMax, m3dV *vClosest, float *dist, int *faceNmb);
} m3dBOX;
