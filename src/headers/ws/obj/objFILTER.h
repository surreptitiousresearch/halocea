#pragma once
// ws-engine obj: abstract predicate base used to select objOBJ nodes (e.g. objOBJ::_CalcBBox /
// CalcBBox_FILTER). DB-verified layout (types_members objFILTER) — size 4 (vtable pointer only).
// Concrete filters (objFILTER_NAME, objFILTER_ID, objFILTER_FUNC, objFILTER_ANIM_NMB,
// objFILTER_OBJ_STATE, objFILTER_NAME_NOCASE, objFILTER_NAME_LEN, objFILTER_LEAF,
// objFILTER_HIDDEN_NAME, ...) are not expanded here — out of scope for this batch.

struct objOBJ; // ../obj/objOBJ.h — pointer only

// DB-verified layout (types_members objFILTER_vtbl): dtr_objFILTER@0, IsUse@4.
typedef struct objFILTER_vtbl {
    void (__fastcall *dtr_objFILTER)(struct objFILTER *self);
    int  (__fastcall *IsUse)(struct objFILTER *self, objOBJ *obj);
} objFILTER_vtbl;

typedef struct objFILTER {
    objFILTER_vtbl *__vftable; // 0x00

    ~objFILTER(); // 0x82660EE0 — sets the vtable back to objFILTER's own (no fields to release)
    int IsUse(objOBJ *obj); // pure — boundary, implemented per concrete filter
} objFILTER;
