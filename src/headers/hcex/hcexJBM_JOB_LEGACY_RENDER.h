#pragma once
#include "../ws/jbm/jbmJOB.h"
#include "../vidDRIVER_DYNGEOM_BUFFER.h"
/* hcexJBM_JOB_LEGACY_RENDER -- the job-manager job that wraps the legacy Blam renderer
 * (hcex_legacy_render) so it can be scheduled through jbmMANAGER like any other frame-pipeline
 * stage (see CLAUDE.md's Frame Job Pipeline table). One global instance, `hcex_job_legacy_render`.
 * DB-verified layout (types_members hcexJBM_JOB_LEGACY_RENDER): jbmJOB@0 (12B, base) -- no
 * additional fields; size 12. */

struct hcexJBM_JOB_LEGACY_RENDER_vtbl; // boundary -- same 5-slot shape as jbmJOB_vtbl

/* --- vidVBUF_MNG -- full DB vtable/struct layouts (types_members vidVBUF_MNG_vtbl /
 * vidVBUF_MNG / vidMEM_MNG); only the DiscardDynAll slot (index 12, offset 0x30) is called
 * from this job, the rest are boundary slots typed void(self) for slot-count fidelity. --- */
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsCONST_ARRAY.h"
struct vidVBUF_MNG;
struct vidBUFFER;        // boundary
struct vidFVF_DESCR;     // boundary
struct objOBJ;           // boundary
struct apDEFRAG_CHUNK;   // boundary
struct vidMEM_MNG_vtbl;  // boundary

typedef struct vidVBUF_MNG_vtbl
{
    void (*dtr_vidVBUF_MNG)(vidVBUF_MNG *self);                 /* 0x00 */
    void (*ReleaseAll)(vidVBUF_MNG *self);                      /* 0x04 boundary */
    void (*DumpStat)(vidVBUF_MNG *self);                        /* 0x08 boundary */
    void (*AppendDyn)(vidVBUF_MNG *self);                       /* 0x0C boundary */
    void (*LockDyn)(vidVBUF_MNG *self);                         /* 0x10 boundary */
    void (*UnlockDyn)(vidVBUF_MNG *self);                       /* 0x14 boundary */
    void (*GetDataAlign)(vidVBUF_MNG *self);                    /* 0x18 boundary */
    void (*CreateBuffer)(vidVBUF_MNG *self);                    /* 0x1C boundary */
    void (*DestroyBuffer)(vidVBUF_MNG *self);                   /* 0x20 boundary */
    void (*ReserveVBDyn)(vidVBUF_MNG *self);                    /* 0x24 boundary */
    void (*Defrag)(vidVBUF_MNG *self);                          /* 0x28 boundary */
    void (*CreateDynamicBufs)(vidVBUF_MNG *self);               /* 0x2C boundary */
    void (*DiscardDynAll)(vidVBUF_MNG *self);                   /* 0x30 (index 12) -- called here */
    void (*GetDynHwB)(vidVBUF_MNG *self);                       /* 0x34 boundary */
    void (*AllocStatData)(vidVBUF_MNG *self);                   /* 0x38 boundary */
    void (*AllocStatDataImpl)(vidVBUF_MNG *self);               /* 0x3C boundary */
    void (*Defrag_AddChunk)(vidVBUF_MNG *self);                 /* 0x40 boundary */
    void (*Defrag_RemoveUnused)(vidVBUF_MNG *self);             /* 0x44 boundary */
} vidVBUF_MNG_vtbl;

/* types_members vidMEM_MNG (size 52) */
typedef struct vidMEM_MNG
{
    vidMEM_MNG_vtbl *__vftable;               /* 0x00 */
    dsVECTOR<vidBUFFER *, 8> buffers;         /* 0x04 */
    dsCONST_ARRAY<vidBUFFER *, 6> curDynBuf;  /* 0x18 */
} vidMEM_MNG;

/* types_members vidVBUF_MNG (size 932) */
struct vidVBUF_MNG : vidMEM_MNG
{
    vidFVF_DESCR *fvfCacheLastUsed[5];        /* 0x034 */
    vidFVF_DESCR *fvfList[200];               /* 0x048 */
    dsVECTOR<objOBJ *, 8> objList;            /* 0x368 */
    dsVECTOR<apDEFRAG_CHUNK *, 8> chunks;     /* 0x37C */
    dsVECTOR<apDEFRAG_CHUNK *, 8> chunkUsed;  /* 0x390 -- size 932 */
};

extern vidDRIVER  *vidDriver;
extern vidVBUF_MNG *vidVBMng;
extern "C" vidDRIVER_DYNGEOM_CHUNK *hcex_dyn_chunk;

/* Legacy Blam renderer entry point (Blam engine boundary -- see CLAUDE.md). */
extern "C" void hcex_legacy_render(void);

/* Xenon rasterizer FX-texture release (name preserved verbatim from the database, including its
 * "raserizer" typo). */
extern "C" void raserizer_release_fx_textures(void); // real return type HRESULT; result discarded here

struct vidLOCK; // ws-engine vid -- full type in src/headers/ws/vid/vidLOCK.h

struct hcexJBM_JOB_LEGACY_RENDER : jbmJOB
{
    static hcexJBM_JOB_LEGACY_RENDER_vtbl vftable; // `hcexJBM_JOB_LEGACY_RENDER::`vftable'' -- boundary

    // 0x823F08DC -- default-construct: name "legacy_render", own vtable, clear state.
    hcexJBM_JOB_LEGACY_RENDER();

    // 0x823EF6B8 -- thunk: re-point the vtable back to the jbmJOB base before base teardown.
    ~hcexJBM_JOB_LEGACY_RENDER();

    // 0x823F090C -- `scalar deleting destructor': re-point the vtable to jbmJOB's, then
    // conditionally operator-delete `this` when bit 0 of deleteFlags is set.
    hcexJBM_JOB_LEGACY_RENDER *ScalarDeletingDtor(unsigned char deleteFlags);

    // 0x823EF6F8 -- run the legacy Blam renderer under vidLock: hcex_legacy_render(), release the
    // rasterizer's FX textures, close out any still-open dynamic-geometry chunk, then discard the
    // frame's dynamic vertex/index buffers.
    void Execute(int threadID);
};

extern hcexJBM_JOB_LEGACY_RENDER hcex_job_legacy_render;
