#pragma once
#include "hcexHALO_OBJECT.h"
struct animINST;

// HCEX bridge: hcexHALO_OBJECT specialization for standalone Halo model entities — applies the
// fixed HCEX-to-Halo coordinate-convention correction (scale + 90-degree yaw) on top of the base
// region-substitution hook. DB-verified layout (types_members hcexHALO_MODEL): base
// hcexHALO_OBJECT @0 — size 4 (no extra fields).
struct animINST;
typedef struct hcexHALO_MODEL_vtbl {
    void (*OnLoadInst)(struct hcexHALO_MODEL *self, animINST *pInst);        // 0x00
    void (*dtr_hcexHALO_MODEL)(struct hcexHALO_MODEL *self, int freeMemory); // 0x04 deleting dtor
} hcexHALO_MODEL_vtbl;

struct hcexHALO_MODEL : hcexHALO_OBJECT {
    static hcexHALO_MODEL_vtbl vftable; // `hcexHALO_MODEL::`vftable'' — boundary, not decompiled

    // 0x823D33F8 — install this class's vftable (no other state to init).
    hcexHALO_MODEL();

    // 0x823D1230 — reset __vftable to the entMANAGER_HANDLER base slot (no owned state to release).
    ~hcexHALO_MODEL();

    // 0x823D342C — compiler-generated scalar deleting destructor (`??_G` mangle).
    hcexHALO_MODEL *scalarDeletingDtor(unsigned char deleteFlags);

    // 0x823D6F4C (overrides hcexHALO_OBJECT::OnLoadInst) — run the base region-substitution hook,
    // then apply the fixed HCEX model->Halo unit conversion: uniform scale by 0.03048
    // (inches-per-Halo-world-unit) and a -90 degree yaw (RotateY), both appended (operOrder=1)
    // to the instance's existing transform.
    void OnLoadInst(animINST *pInst); // base OnLoadInst is a vtable slot, not a C++ virtual: no 'override'
};
