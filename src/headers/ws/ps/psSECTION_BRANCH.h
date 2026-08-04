#pragma once
// ws-engine ps: a section tree/branch — the ref-counted owner of a top-level section body and
// the flags governing its deletion/rescan state.
// DB-verified layout (types_members psSECTION_BRANCH): refCount@0, pTopSection@4,
//   (anonymous):6 pad @8, isDeleting:1 @8, isNeedRescan:1 @8 — size 9.

struct psSECTION_DATA; // top section body (full layout in psSECTION_DATA.h)

typedef struct psSECTION_BRANCH {
    int              refCount;         // 0x00
    psSECTION_DATA  *pTopSection;      // 0x04
    unsigned char    _reserved : 6;    // 0x08 (DB anonymous 6-bit pad)
    unsigned char    isDeleting : 1;   // 0x08 (raw mask 0x40 on this ABI)
    unsigned char    isNeedRescan : 1; // 0x08

    // 0x82519180 — recursively clear the parent lists of `pData` and every sub-section.
    void RecClearParents(psSECTION_DATA *pData); // protected (IAA)

    // ?RecRescan@psSECTION_BRANCH@@IAAXPAVpsSECTION_DATA@@@Z — recursively re-scan `pData` and its
    // sub-sections after a pending-rescan flag was consumed.
    void RecRescan(psSECTION_DATA *pData);       // boundary — protected (IAA)

    // 0x82519A90 — mark deleting, clear parents, destroy the top section, and free this branch.
    void Delete();
} psSECTION_BRANCH;
