#pragma once

#include "txmTEXTURE.h"
#include "txmTEXTURE_PTR.h"
#include "txmRTMGR_ENTRY.h"
#include "../ap/apSTATE_T.h"
#include "../ds/dsVECTOR.h"

/* txmMANAGER -- ws-engine texture manager (txm subsystem). DB-verified layout (types_members
 * txmMANAGER, size 0x3A0 (928)). listTex/listNeedSort were added by the Alloc() reconstruction
 * (DB-verified offsets 0x150/0x164); the rest of the trailing fallback-texture block is still
 * raw padding -- see [[re-source-boundary-conventions]]. */

// Stateless ascending-order comparator for txmMANAGER::listTex, used by Alloc()'s InsertSorted
// call. DB type has no data members (types_members txmCMP_TEX_SORT: 0 rows). boundary (ordering
// predicate body external to this batch).
typedef struct txmCMP_TEX_SORT {
} txmCMP_TEX_SORT;

// Stateless key comparator used by txmMANAGER::Find's FindSorted: orders a txmTEXTURE* against a
// const char* name (name-keyed binary search over the sorted listTex). DB type has no data
// members (types txmCMP_TEX_FIND: size 1, empty). boundary (predicate body external to this batch).
typedef struct txmCMP_TEX_FIND {
} txmCMP_TEX_FIND;

struct fioFILE_MEM;     // ../fio/fioFILE_MEM.h — LoadFromPak param only (pointer)
struct txmMANAGER_vtbl; // boundary — slot table not expanded; MakeTex documented below

typedef struct txmMANAGER
{
    txmMANAGER_vtbl             *__vftable;  /* 0x000 */
    char                        path[260];   /* 0x004 */
    char                        ext[64];     /* 0x108 */
    int                          noFailAdd;    /* 0x148 */
    int                          mergeNMSM;    /* 0x14C */
    dsVECTOR<txmTEXTURE *, 8>    listTex;      /* 0x150 */
    int                          listNeedSort; /* 0x164 */
    int                          currFrame;    /* 0x168 */
    apSTATE_T<unsigned long>     state;        /* 0x16C */
    int                          usePaks;      /* 0x170 */
    txmTEXTURE_PTR                whiteTex;         /* 0x174 */
    txmTEXTURE_PTR                blackTex;         /* 0x178 */
    txmTEXTURE_PTR                redTex;           /* 0x17C */
    txmTEXTURE_PTR                colorTex;         /* 0x180 */
    txmTEXTURE_PTR                normalMapTex;     /* 0x184 */
    txmTEXTURE_PTR                flatDirLMTex;     /* 0x188 */
    txmTEXTURE_PTR                whiteDifLMTex;    /* 0x18C */
    txmTEXTURE_PTR                gammaLookup;      /* 0x190 */
    txmTEXTURE_PTR                normalMapZLookup; /* 0x194 */
    txmTEXTURE_PTR                msaaRemapTable;   /* 0x198 */
    txmTEXTURE_PTR                ssaoNoiseTex;     /* 0x19C */
    txmTEXTURE_PTR                fogNoiseTex;      /* 0x1A0 */
    txmTEXTURE_PTR                mlaaLookup;       /* 0x1A4 */
    txmRTMGR_ENTRY                rtMgrTargets[25]; /* 0x1A8 */
    int                           fs_msaa_remap_index; /* 0x39C */

    ~txmMANAGER(); // boundary -- dtr_txmMANAGER (vtable slot 0; vptr modeled as __vftable above)

    // 0x826F71F0 -- reversed in txmMANAGER__Load.cpp. Non-virtual (not in txmMANAGER_vtbl).
    int Load(txmTEXTURE *pTex, int isTryLoadNotFound);

    // vtable (protected, MAA mangle) -- allocate a fresh derived txmTEXTURE (e.g. txmD3D_TEX) and
    // initialise its name/flags. boundary (external to this batch; called from Alloc).
    txmTEXTURE *MakeTex(const char *name, int texFlags);

    // 0x826FB0CC -- reversed in txmMANAGER__Alloc.c. Allocates a texture via MakeTex, then
    // records it in listTex: appended unsorted if listNeedSort is set, else spliced into its
    // sorted position (dup-ignore) via InsertSorted<txmCMP_TEX_SORT>.
    txmTEXTURE *Alloc(const char *name, int texFlags);

    // 0x826FAFD8 -- reversed in txmMANAGER__Find.cpp. Name-keyed lookup in listTex: lazily
    // Sort()s and trims trailing-null slots when listNeedSort is set, then FindSorted() by name.
    // Returns nullptr on a miss.
    txmTEXTURE *Find(const char *name);

    // 0x826FBDD0 -- reversed in txmMANAGER__Add.cpp. Get-or-create the named texture, filling its
    // descriptor (default-desc table or on-disk ".td") and folding in the requested state flags.
    txmTEXTURE *Add(const char *name, unsigned int flags, int checkFile);

    // Load a texture's pixel data from an already-opened pak/memory stream. boundary (called from
    // package::GS_PACKAGE::ProcessLoadTextures; body external to this batch).
    void LoadFromPak(txmTEXTURE *pTex, fioFILE_MEM *pFile, int flags);

    // Lightweight cursor over the live entries of listTex. DB-verified layout
    // (types_members txmMANAGER::ITERATOR, size 8): index@0x00, listTex@0x04.
    struct ITERATOR {
        int                         index;    /* 0x00 current live-texture index */
        dsVECTOR<txmTEXTURE *, 8>  *listTex;  /* 0x04 backing texture vector */

        // ?operator++@ITERATOR@txmMANAGER@@ -- advance to the next live texture. boundary
        void operator++();
    };

    // ?Begin@txmMANAGER@@QAA?AVITERATOR@1@XZ -- construct an iterator at the first live texture.
    // boundary (body external to this batch).
    ITERATOR Begin();
} txmMANAGER;

extern txmMANAGER *txmManager;
