#pragma once
// ws-engine ap: localization base class. DB-verified layout (types_members apL10N,
// ordinal 20076): a single vtable pointer at offset 0, size 4 — apL10N itself carries
// no additional data members in this binary. Methods not decompiled here; boundary.

struct apL10N;

// DB-verified layout (types_members apL10N_vtbl): 3 slots, 12 bytes.
typedef struct apL10N_vtbl {
    void           (*dtr_apL10N)(apL10N *self);                                     // 0x00
    const wchar_t *(*GetStringById)(apL10N *self, const char *id);                  // 0x04
    const char    *(*GetUTFStringById)(apL10N *self, const char *id);               // 0x08
} apL10N_vtbl;

typedef struct apL10N {
    apL10N_vtbl *__vftable; // 0x00

    // boundary — apL10N's own method bodies are not decompiled as part of this task.
} apL10N;
