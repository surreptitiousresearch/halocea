#pragma once
// ws-engine inst: abstract instance-manager handler base (registers with instMANAGER to
// intercept scene-instance creation/management). DB-verified layout (types_members
// instMANAGER_HANDLER) — size 4 (vtable pointer only). Out of scope for this batch beyond the
// vtable slots needed to call through a derived class's __vftable.
// DEVIATION (2026-07 haloTerm re-source pass): a fresh types_members query on
// instMANAGER_HANDLER_vtbl reports IsInstManagable@0 / dtr_instMANAGER_HANDLER@4 — this
// re-orders the previous single-slot layout (which had the dtor at offset 0). Disasm at
// haloTerm (0x823CF1B8) confirms both facts at once: the dtor call loads its function pointer
// from `vftable+4` (not +0), and passes a second arg (`li r4,1`) — so the DB's slot *order* is
// now trusted but its dtor *arity* (shown void(self) with no args) is corrected back to
// (self, int freeMemory) per the disassembly, matching the original arg-count finding.
struct scnINST_CREATE_DATA; // boundary — not decompiled here

typedef struct instMANAGER_HANDLER_vtbl {
    bool (*IsInstManagable)(struct instMANAGER_HANDLER *self, scnINST_CREATE_DATA *data); // 0x00
    void (*dtr_instMANAGER_HANDLER)(struct instMANAGER_HANDLER *self, int freeMemory);    // 0x04 deleting dtor
} instMANAGER_HANDLER_vtbl;

typedef struct instMANAGER_HANDLER {
    static instMANAGER_HANDLER_vtbl vftable; // `instMANAGER_HANDLER::`vftable'' — boundary, not decompiled
    instMANAGER_HANDLER_vtbl *__vftable;     // 0x00

    instMANAGER_HANDLER();  // 0x823CE828 — boundary, external to this batch
    ~instMANAGER_HANDLER(); // 0x823CE870 — boundary, external to this batch
} instMANAGER_HANDLER;
