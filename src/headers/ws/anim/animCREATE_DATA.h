#pragma once
#include "../m3d/m3dMATR.h"
#include "../ps/psSECTION.h"
#include "../ds/dsAFFIX_STRING.h"
#include "../rend/rendSR_DATA.h"
// ws-engine anim: parameter block passed to animINST creation.
// DB-verified layout (types_members animCREATE_DATA) — size 232 (0xE8).

struct animCREATE_DATA_vtbl; // polymorphic base vtable (pointer only)  boundary

typedef struct animCREATE_DATA {
    static animCREATE_DATA_vtbl vftable; // `animCREATE_DATA::`vftable'' -- boundary, not decompiled
    animCREATE_DATA_vtbl *__vftable;      // 0x00
    m3dMATR               matrInst;       // 0x04 initial instance transform
    float                 frameCur;       // 0x44
    int                   state;          // 0x48
    psSECTION             ps;             // 0x4C
    dsAFFIX_STRING        affixes;        // 0x50
    char                  name[128];      // 0x54
    __int16               animSeqNmb;     // 0xD4
    unsigned __int8       animSeqIsCycled;// 0xD6
    unsigned __int8       isIgnorePS_Scale;// 0xD7
    rendSR_DATA          *srData;         // 0xD8 optional shader-replacement data
    float                 scaleX;         // 0xDC
    float                 scaleY;         // 0xE0
    float                 scaleZ;         // 0xE4

    // 0x8252DDE0 — default-construct ps/affixes, clear srData, then delegate to Clear() for the
    // remaining default field values (identity transform, scale 1, ...).
    animCREATE_DATA();

    // 0x8252CEA0 (virtual) — free any attached shader-replacement data, release the affixes
    // buffer reference, and destroy ps.
    virtual ~animCREATE_DATA();

    // 0x8252EF60 — memberwise copy-assign (matrix, frame/state/ps/affixes/name/scale, and a
    // deep copy of srData if the source has one).
    animCREATE_DATA &operator=(const animCREATE_DATA &src);

    // 0x8252BF08 — if srData is set, copy it into the caller-supplied record.
    void ApplySRData(rendSR_DATA *_srData);

    // 0x8252D3A8 — reset every field to its default (identity transform, scale 1, empty name/ps/
    // affixes, no sequence) and release any attached shader-replacement data.
    void Clear();
} animCREATE_DATA;
