#pragma once
#include "m3dV.h"
#include "m4dV.h"
// ws-engine m3d math: 4x4 matrix.
// DB-verified layout (types_members m3dMATR / anon union $49ACCF...): a single 64-byte
// union aliasing elements[16], the named-cell __s1 view, m[4][4], and v[4] (m4dV).

/* DB anonymous struct _40942CBCB8F0A1CDBC81269929B28324 — the D3D-style named-cell view
 * ($-names kept verbatim; $ in identifiers is an MSVC extension). */
typedef struct _40942CBCB8F0A1CDBC81269929B28324 {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
} _40942CBCB8F0A1CDBC81269929B28324;

/* DB anonymous union _49ACCF28C24620D6F14D463E664D61D1 (64 bytes) — the matrix storage. */
typedef union _49ACCF28C24620D6F14D463E664D61D1 {
    float elements[16];                             /* 0x00 */
    struct _40942CBCB8F0A1CDBC81269929B28324 __s1;  /* 0x00 named-cell view */
    float m[4][4];                                  /* 0x00 */
    m4dV  v[4];                                     /* 0x00 */
} _49ACCF28C24620D6F14D463E664D61D1;

typedef struct m3dMATR {
    // DB anonymous union made anonymous here so elements/m/v/__s1 are accessible directly
    // (layout-neutral: single 64-byte union at offset 0, same as the named ___u0 member).
    union {
        float elements[16];                             /* 0x00 */
        struct _40942CBCB8F0A1CDBC81269929B28324 __s1;  /* 0x00 named-cell view */
        float m[4][4];                                  /* 0x00 */
        m4dV  v[4];                                     /* 0x00 */
    };

    // 0x82541F10 — (float,float,float,int) overload (base name, reversed in this subsystem).
    void Translate(float x, float y, float z, int oper);
    // 0x82542AA8 — const m3dV* + int overload (thin forwarder, reversed as Translate__2).
    void Translate(const m3dV *v, int oper);
    // 0x823CD458
    void GetOrigin(m3dV *org) const;
    // 0x825429C8
    void Identity();
    // 0x825449B8
    void RemoveScale();
    // 0x82541C48
    void TransformPoint(const m3dV *p, m3dV *pResult) const;
    // ?TransformPoint@m3dMATR@@QBAXPAUm3dV@@@Z — in-place overload (transform p in place).
    void TransformPoint(m3dV *p) const;
    // 0x82544A68
    void TransformPointList(int nPoint, void *srcPtr, int srcStride, void *dstPtr, int dstStride) const;
    // 0x82542C28
    void Transform(const m3dMATR *pMTrans, int oper);
    // 0x825432A0 ?MakeRotY@m3dMATR@@QAAXM@Z — build a rotation matrix about the Y axis (degrees).
    void MakeRotY(float angleDeg);
    // 0x82541FD0
    void MakeLCS2WCS(const m3dV *org, m3dV *vx, m3dV *vy, m3dV *vz);
    // 0x82542F10 — inverse per-axis scale factors (1/length of each basis row).
    void GetInvScale(float *invScaleX, float *invScaleY, float *invScaleZ) const;
    // 0x825423A8 — per-axis scale factors (length of each basis row).
    void GetScale(float *scaleX, float *scaleY, float *scaleZ) const;
    // 0x82542AC0 — scale the matrix; `oper` selects column/world (0,2) vs row/local (1) scaling.
    void Scale(float sx, float sy, float sz, int oper);
    // 0x82544CC0 — build a rotation matrix from XYZ Euler angles (degrees).
    void MakeEuler(float angleX, float angleY, float angleZ);
    // 0x82543C00 — LCS-to-WCS with `dir` as the Z (forward) axis; up derived from world Y/X.
    void _MakeLCS2WCS_VZ(m3dV *org, m3dV *dir);
    // 0x82543D30 — LCS-to-WCS with `dir` as the Y (up) axis; forward derived from world Y/X.
    void _MakeLCS2WCS_VY(m3dV *org, m3dV *dir);
    // 0x82546188 — validate: finite elements, uniform scale, orthonormal-ish axes.
    int Check() const;
    // 0x82544630 — invert into destMatr; returns 0 if singular (|det| < 1e-14).
    int Invert(m3dMATR *destMatr) const;
    // 0x82542D08 — decompose the rotation into XYZ Euler angles (degrees), with the classic
    // gimbal-lock branch when |elements[2]| ~= 1.
    void GetRotateRot(float *rotX, float *rotY, float *rotZ) const;
} m3dMATR;

// 0x825426E8 — out = a * b (row-major 4x4 matrix product): out.row[i] = sum_j a.row[i][j] * b.row[j].
void m3dMultiplyMatr(const m3dMATR *a, const m3dMATR *b, m3dMATR *out);
