#pragma once
#include "m3dV.h"
#include "m3dBOX.h"
// ws-engine m3d math: oriented bounding box.
// DB-verified layout (types_members m3dOBB): org@0, vx@12, vy@24, vz@36 (all m3dV),
// sx@48, sy@52, sz@56 (all float) — size 60.

typedef struct m3dOBB {
    m3dV  org;
    m3dV  vx;
    m3dV  vy;
    m3dV  vz;
    float sx;
    float sy;
    float sz;

    // 0x82A6F248 — seed org/vx/vy/vz from an origin + two basis vectors (third derived).
    // ?CalcStart@m3dOBB@@QAAXPAUm3dV@@00@Z  boundary
    void CalcStart(m3dV *org, m3dV *vx, m3dV *vy);
    // 0x825E0DB0 — length of the extents vector sqrt(sx^2+sy^2+sz^2).
    float GetSize();
    // 0x82A70FA8
    void CalcEnclBox(m3dBOX *bb) const;
    // 0x82A6EE58 — emit the 8 box corners (org + scaled axis combinations).
    void GetVertices(m3dV *verts) const;
    // 0x82A6F0C0 — inflate the box by `value` on every axis, recentring the origin.
    void Expand(float value);

    // Recompute this OBB to tightly enclose `count` world-space vertices. boundary — not
    // reversed in this batch (used by aiPLANNER::NoticeFlyObject).
    void CalcUpdate(const m3dV *verts, int count);

    // 0x82702F60 — build the world-to-local matrix for this OBB into `pMatr`; when `pMatrInv`
    // is non-null also emit the local-to-world (inverse) matrix, and fill `bbLCS` with the box
    // in local space (the axis-aligned extents). const. boundary — not reversed in this batch.
    void GetMatrW2L(m3dMATR *pMatr, m3dMATR *pMatrInv, m3dBOX *bbLCS) const; // boundary

    // 0x82A70728 (?IsBelongPoint@m3dOBB@@QBAHPBUm3dV@@@Z) — const: nonzero when `point` lies
    // inside the oriented box (projects the point onto each axis and range-checks the extents).
    int IsBelongPoint(const m3dV *point) const;

    // 0x82A70DFC — closest-point / distance test of `point` against the OBB, early-out when the
    // distance exceeds `distMax`. When `vClosest` is non-null the closest point is written (in
    // world space) and returned nonzero when within range; `dist` receives the distance and
    // `faceNmb` the face index. REVERSED: src/ws/m3d/m3dOBB__DistPoint.cpp.
    // DEVIATION: an earlier pass declared this as (m4dV*,float,m3dV*,void*,float*); the mangled
    // name (PBUm3dV MPAU2 PAM PAH) + disasm prove it is (const m3dV*,float,m3dV*,float*,int*).
    int DistPoint(const m3dV *point, float distMax, m3dV *vClosest, float *dist, int *faceNmb);
} m3dOBB;
