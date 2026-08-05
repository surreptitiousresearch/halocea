#pragma once
// ws-engine ent: abstract entity-manager handler base (registers with entMANAGER to intercept
// entity/animINST load-instance events, e.g. hcexHALO_OBJECT::OnLoadInst). DB-verified layout
// (types_members entMANAGER_HANDLER) — size 4 (vtable pointer only). Out of scope for this batch
// beyond the vtable slot needed to reset a derived class's __vftable in its destructor path.

struct animINST; // ../anim/animINST.h (pointer only)

typedef struct entMANAGER_HANDLER_vtbl {
    void (*OnLoadInst)(struct entMANAGER_HANDLER *self, animINST *pInst);            // 0x00 boundary
    void (*dtr_entMANAGER_HANDLER)(struct entMANAGER_HANDLER *self, int freeMemory); // 0x04 deleting dtor
} entMANAGER_HANDLER_vtbl;

typedef struct entMANAGER_HANDLER {
    static entMANAGER_HANDLER_vtbl vftable; // `entMANAGER_HANDLER::`vftable'' — boundary, not decompiled
    entMANAGER_HANDLER_vtbl *__vftable;     // 0x00

    entMANAGER_HANDLER();  // 0x823CEBB8 — boundary, external to this batch

protected:
    ~entMANAGER_HANDLER(); // 0x823CEB70 (protected per `MAA` mangle) — boundary, not decompiled
} entMANAGER_HANDLER;
