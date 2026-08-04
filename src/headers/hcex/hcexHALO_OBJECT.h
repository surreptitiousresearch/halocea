#pragma once
#include "../ws/ent/entMANAGER_HANDLER.h"
struct animINST;

// HCEX bridge: entMANAGER_HANDLER that hooks animINST load-instance notifications for Halo-model
// entities, wiring the ws-engine's "permutations" region-substitution mechanism onto Blam-driven
// objects. DB-verified layout (types_members hcexHALO_OBJECT): base entMANAGER_HANDLER @0 — size 4
// (no extra fields).
struct animINST;
typedef struct hcexHALO_OBJECT_vtbl {
    void (*OnLoadInst)(struct hcexHALO_OBJECT *self, animINST *pInst);         // 0x00
    void (*dtr_hcexHALO_OBJECT)(struct hcexHALO_OBJECT *self, int freeMemory); // 0x04 deleting dtor
} hcexHALO_OBJECT_vtbl;

struct hcexHALO_OBJECT : entMANAGER_HANDLER {
    static hcexHALO_OBJECT_vtbl vftable; // `hcexHALO_OBJECT::`vftable'' — boundary, not decompiled

    // 0x823D1240 — install this class's vftable (no other state to init).
    hcexHALO_OBJECT();

    // 0x823CEBD0 — reset __vftable to the entMANAGER_HANDLER base slot (no owned state to release).
    ~hcexHALO_OBJECT() override;

    // 0x823D12B4 — compiler-generated scalar deleting destructor (`??_G` mangle).
    hcexHALO_OBJECT *scalarDeletingDtor(unsigned char deleteFlags);

    // 0x823D6BA0 (overrides entMANAGER_HANDLER's load-instance hook) — after the base engine's
    // pre-init handling, mark the instance initialized (state |= 2) and walk its "permutations"
    // section: for each key (a region name), find the matching object on the instance by
    // case-insensitive name; if found, resolve the key's string value (a substitute region/object
    // name) and find it under that region (case-insensitive); when both resolve, toggle process
    // visibility (region off, substitute on); log a diagnostic when either lookup misses.
    void OnLoadInst(animINST *pInst); // base OnLoadInst is a vtable slot, not a C++ virtual: no 'override'
};
