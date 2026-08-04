#pragma once
#include "../ap/apSTATE_T.h"
// ws-engine m3d: keyframed spline evaluator. DB-verified layout (types_members m3dSPL /
// m3dSPL_vtbl). GetValue and the other evaluators are dispatched through the vtable.

struct m3dSPL;

// Virtual dispatch table. DB-verified (types_members m3dSPL_vtbl).
struct m3dSPL_vtbl {
    void     (__fastcall *dtr_m3dSPL)(m3dSPL *self, int deleteFlag); // scalar-deleting dtr; deleteFlag bit0 => free
    // Evaluate the spline. Real call sites pass (this, keyframeIdx, float *out, 0, sampleTime);
    // the extra args are absorbed by the PPC varargs ABI.
    void     (__fastcall *GetValue)(m3dSPL *self, float t, void *out, void *ctx);
    void     (__fastcall *InterpolateKp)(m3dSPL *self, void *a, void *b, float t, void *out);
    void     (__fastcall *GetValueClosest)(m3dSPL *self, float t, void *out, void *ctx);
    float    (__fastcall *DistPoint)(m3dSPL *self, void *pt, int *idx, float *dist, void *ctx);
    m3dSPL  *(__fastcall *Convert)(m3dSPL *self, int type, int dim);
};

struct m3dSPL {
    m3dSPL_vtbl              *__vftable;  // 0x00
    apSTATE_T<unsigned char>  state;      // 0x04
    unsigned __int8           type;       // 0x05
    unsigned __int8           valDim;     // 0x06
    unsigned __int8           dataDim;    // 0x07
    int                       nKp;        // 0x08 keyframe count
    int                       dataSize;   // 0x0C
    unsigned __int8          *pData;      // 0x10

    // argument (parametric time) of keyframe `idx`. boundary
    float GetKpArg(int idx);

    // 0x82A28348 (?GetKpValue@m3dSPL@@QBAXHPAX@Z) — const: write the value of keyframe `idx` into
    // `out` (untyped; dim-dependent). boundary.
    void GetKpValue(int idx, void *out) const;

    // Static factory readers (DB: ?Read@m3dSPL@@SA..., ?ReadOld@m3dSPL@@SA...).
    static m3dSPL *Read(struct fioFILE *file);
    static m3dSPL *ReadOld(int type, struct fioFILE *file);
};
