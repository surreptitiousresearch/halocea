#pragma once
#include "../ws/jbm/jbmJOB.h"
#include "../vidDRIVER_DYNGEOM_BUFFER.h"
/* hcexJBM_JOB_LEGACY_RENDER -- the job-manager job that wraps the legacy Blam renderer
 * (hcex_legacy_render) so it can be scheduled through jbmMANAGER like any other frame-pipeline
 * stage (see CLAUDE.md's Frame Job Pipeline table). One global instance, `hcex_job_legacy_render`.
 * DB-verified layout (types_members hcexJBM_JOB_LEGACY_RENDER): jbmJOB@0 (12B, base) -- no
 * additional fields; size 12. */

struct hcexJBM_JOB_LEGACY_RENDER_vtbl; // boundary -- same 5-slot shape as jbmJOB_vtbl

/* --- vidVBUF_MNG -- DB vtable layout (types_members vidVBUF_MNG_vtbl). Only the DiscardDynAll
 * slot (index 12, offset 0x30) is called from this job; every other slot carries its real DB
 * signature, cross-checked against the callee's mangled symbol (see the per-slot comments).
 * DEVIATION: the slots used to be declared void(vidVBUF_MNG*) "for slot-count fidelity", which
 * made 14 of the 18 signatures wrong -- a stub vtable that compiled because nothing but slot 12
 * is ever called through it. The vidVBUF_MNG / vidMEM_MNG STRUCT layouts used to be
 * re-declared below this vtable, which is `error: redefinition` against their canonical headers in
 * the header_layout probe TU. Both copies were data-member-identical to the canonicals
 * (vidMEM_MNG: __vftable@0, buffers@4, curDynBuf@0x18, size 52; vidVBUF_MNG: vidMEM_MNG base,
 * fvfCacheLastUsed@0x34, fvfList@0x48, objList@0x368, chunks@0x37C, chunkUsed@0x390, size 932 --
 * both re-confirmed against `types`/`types_members` while merging), and the canonicals are the
 * supersets: they additionally declare vidMEM_MNG::GetDynBuf and vidVBUF_MNG's
 * AllocStatData/AllocStatDataImpl/WriteStatData/FitFVF. `__vftable` stays typed as the
 * vidMEM_MNG_vtbl base there too, which is what hcexJBM_JOB_LEGACY_RENDER.cpp's documented
 * reinterpret_cast to vidVBUF_MNG_vtbl* already relies on. --- */
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsCONST_ARRAY.h"
#include "../ws/vid/vidVBUF_MNG.h"   /* canonical vidVBUF_MNG + (via it) vidMEM_MNG */
struct vidBUFFER;        // boundary
struct vidFVF_DESCR;     // boundary
struct objOBJ;           // boundary
struct apDEFRAG_CHUNK;   // boundary
struct vidMEM_MNG_vtbl;  // boundary
struct scnSCENE;         // boundary -- full type in src/headers/ws/scn/scnSCENE.h

typedef struct vidVBUF_MNG_vtbl
{
    /* ??1vidVBUF_MNG@@UAA@XZ */
    void             (*dtr_vidVBUF_MNG)(vidVBUF_MNG *self);                        /* 0x00 */
    /* ?ReleaseAll@vidMEM_MNG@@UAAXXZ */
    void             (*ReleaseAll)(vidVBUF_MNG *self);                             /* 0x04 */
    /* ?DumpStat@vidMEM_MNG@@UAAXHPBD@Z */
    void             (*DumpStat)(vidVBUF_MNG *self, int detail, const char *tag);   /* 0x08 */
    /* ?AppendDyn@d3dVBUF_MNG@@UAAHW4vidBUF_CONTENT@@PBXHK@Z */
    int              (*AppendDyn)(vidVBUF_MNG *self, vidBUF_CONTENT cont,
                                  const void *data, int size, unsigned int align); /* 0x0C */
    /* ?LockDyn@d3dVBUF_MNG@@UAAHPAPAXW4vidBUF_CONTENT@@HK@Z */
    int              (*LockDyn)(vidVBUF_MNG *self, void **ppData, vidBUF_CONTENT cont,
                                int size, unsigned int align);                     /* 0x10 */
    /* ?UnlockDyn@d3dVBUF_MNG@@UAAXW4vidBUF_CONTENT@@H@Z */
    void             (*UnlockDyn)(vidVBUF_MNG *self, vidBUF_CONTENT cont, int size); /* 0x14 */
    /* ?GetDataAlign@vidMEM_MNG@@MBAHW4vidBUF_CONTENT@@@Z -- const member; the DB row types the
     * slot's `this` non-const and this table follows the DB. */
    int              (*GetDataAlign)(vidVBUF_MNG *self, vidBUF_CONTENT cont);      /* 0x18 */
    /* ?CreateBuffer@d3dVBUF_MNG@@UAA_NW4vidBUF_CONTENT@@V?$apSTATE_T@E@@AAPAUvidBUFFER@@H@Z
     * DEVIATION: `AAPAUvidBUFFER@@` is `vidBUFFER *&`; the DB row spells the same ABI slot
     * `vidBUFFER **` and this table follows the DB. */
    bool             (*CreateBuffer)(vidVBUF_MNG *self, vidBUF_CONTENT cont,
                                     apSTATE_T<unsigned char> flags,
                                     vidBUFFER **ppBuffer, int size);              /* 0x1C */
    /* ?DestroyBuffer@d3dVBUF_MNG@@UAAXPAUvidBUFFER@@@Z */
    void             (*DestroyBuffer)(vidVBUF_MNG *self, vidBUFFER *pBuffer);      /* 0x20 */
    /* ?ReserveVBDyn@vidVBUF_MNG@@UAAXH@Z */
    void             (*ReserveVBDyn)(vidVBUF_MNG *self, int size);                 /* 0x24 */
    /* ?Defrag@vidVBUF_MNG@@UAAXPAVscnSCENE@@_N@Z */
    void             (*Defrag)(vidVBUF_MNG *self, scnSCENE *pScene, bool force);   /* 0x28 */
    /* ?CreateDynamicBufs@d3dVBUF_MNG@@UAA_NXZ */
    bool             (*CreateDynamicBufs)(vidVBUF_MNG *self);                      /* 0x2C */
    /* ?DiscardDynAll@d3dVBUF_MNG@@UAAXXZ -- index 12, the only slot called from this job */
    void             (*DiscardDynAll)(vidVBUF_MNG *self);                          /* 0x30 */
    /* ?GetDynHwB@d3dVBUF_MNG@@UBAPAXW4vidBUF_CONTENT@@@Z -- const member, see GetDataAlign */
    void            *(*GetDynHwB)(vidVBUF_MNG *self, vidBUF_CONTENT cont);         /* 0x34 */
    /* ?AllocStatData@vidVBUF_MNG@@UAAPAUvidDEFRAG_CHUNK@@W4vidBUF_CONTENT@@V?$apSTATE_T@E@@HPAX@Z
     * `V?$apSTATE_T@E@@` is the class BY VALUE, not a pointer to it. */
    vidDEFRAG_CHUNK *(*AllocStatData)(vidVBUF_MNG *self, vidBUF_CONTENT cont,
                                      apSTATE_T<unsigned char> flags,
                                      int size, void *auxData);                    /* 0x38 */
    /* ?AllocStatDataImpl@d3dVBUF_MNG@@UAAPAUvidDEFRAG_CHUNK@@W4vidBUF_CONTENT@@V?$apSTATE_T@E@@HPAX@Z */
    vidDEFRAG_CHUNK *(*AllocStatDataImpl)(vidVBUF_MNG *self, vidBUF_CONTENT cont,
                                          apSTATE_T<unsigned char> flags,
                                          int size, void *auxData);                /* 0x3C */
    /* ?Defrag_AddChunk@vidVBUF_MNG@@MAAXPAUvidDEFRAG_CHUNK@@@Z */
    void             (*Defrag_AddChunk)(vidVBUF_MNG *self, vidDEFRAG_CHUNK *pChunk); /* 0x40 */
    /* ?Defrag_RemoveUnused@vidVBUF_MNG@@MAAXXZ */
    void             (*Defrag_RemoveUnused)(vidVBUF_MNG *self);                    /* 0x44 */
} vidVBUF_MNG_vtbl;

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
