#pragma once
/* Boundary declarations for hcex_create_effect.c.
 *
 * hcex_create_effect bridges the Blam effect-spawn request into the ws-engine. It leans heavily on the
 * ws-engine template containers (dsTSTRING_flat / dsVECTOR), the sml state system, the snd audio system and the
 * ent/anim object model — all of which are treated as extern boundaries per the project's re-source
 * conventions (declared extern boundaries). Only the fields actually touched by hcex_create_effect are modeled,
 * at their database offsets; the rest is padding. The ws container "methods" are declared as free functions
 * whose first parameter is the object, mirroring the compiler-generated thiscall the decompiler shows. */

#include "hcex_float3.h"
#include "../ws/ds/dsTSTRING.h"            /* canonical dsTSTRING<T> (+ dsTSTRING_BUF_HEADER<T>) */
#include "../ws/ds/dsVECTOR.h"             /* canonical dsVECTOR<T,N> template */
#include "../ws/ds/WEAK_PTR.h"             /* canonical ds::WEAK_PTR<T> */
#include "../ws/ds/dsPAIR.h"               /* canonical dsPAIR<K,V> template */
#include "../ws/anim/animTPL.h"            /* canonical animTPL (name@0x2C etc.) */
#include "../ws/anim/animINST.h"           /* canonical animINST (state2@0x8, pEnt@0xC, pTpl@0x15C) */
#include "../ws/m3d/m3dV.h"                /* m3dV — snd::BUFFER_PARAMS.pos */
#include "../ws/wb/dbgVAR_boundary.h"      /* dbgVAR_SIMPLE<bool,1> debug toggles */

/* dsTSTRING_flat — the HCEX boundary convention's former flat plain-C model of dsTSTRING<char>;
 * now an alias of the canonical template (identical layout: pBuffer@0, 4 bytes). */
typedef dsTSTRING<char> dsTSTRING_flat;

typedef int dsCMP;

#include "../ws/ds/dsSTRID.h"   /* canonical dsSTRID — avoids C2011 vs the prop/ent chain */

/* --- effect record stored in hcexCreateEffects --- */
typedef struct HCEX_EFFECT_LOC
{
    hcex_float3 pos;   /* 0x00 */
    hcex_float3 vec;   /* 0x0C */
    dsTSTRING<char>  name;  /* 0x18 */
} HCEX_EFFECT_LOC;

typedef struct HCEX_EFFECT
{
    dsVECTOR<HCEX_EFFECT_LOC, 8> locations; /* 0x00 */
    dsTSTRING<char>      hcexName;  /* 0x14 */
} HCEX_EFFECT;

/* --- ent / anim object model: canonical animTPL / animINST included above. The former flat
 * partial models (state2/pEnt/pTpl at raw offsets) are superseded by the DB-verified headers. --- */
#include "../ws/ent/entENTITY.h"      /* full entENTITY (pInst@0x15C) — .cpp reads ent->pInst */
typedef struct sml_STATE sml_STATE;   /* opaque; 12 bytes */

typedef struct FP_MODEL
{
    int       plrIdx;           /* 0x00 */
    int       modelId;          /* 0x04 */
    int       modelIdx;         /* 0x08 */
    animINST *pInst;            /* 0x0C */
    int       isActCamo;        /* 0x10 */
    bool      isHiddenCam1;     /* 0x14 */
    bool      isHiddenCam1Prev; /* 0x15 */
    bool      isHiddenCam2;     /* 0x16 */
    bool      isHiddenCam2Prev; /* 0x17 */
} FP_MODEL;

struct m3dMATR; // ../ws/m3d — pointer/vector element only
typedef struct HCEX_OBJ
{
    int           id;            /* 0x00 */
    animINST     *pInst;         /* 0x04 */
    int           modelIdx;      /* 0x08 */
    dsVECTOR<ds::WEAK_PTR<entENTITY>, 8> followers;    /* 0x0C */
    dsVECTOR<m3dMATR, 8>                 followersOfs; /* 0x20 */
    unsigned int  _reserved : 17;        /* 0x34 (DB anonymous 17-bit pad) */
    unsigned int  isFuncDin : 1;         /* 0x36 */
    unsigned int  isFuncCin : 1;
    unsigned int  isFuncBin : 1;
    unsigned int  isFuncAin : 1;
    unsigned int  isFuncD : 1;
    unsigned int  isFuncC : 1;
    unsigned int  isFuncB : 1;
    unsigned int  isFuncA : 1;
    unsigned int  isMachinePositionOn : 1;
    unsigned int  isMachinePowerOn : 1;
    unsigned int  isMachineLocked : 1;
    unsigned int  isShieldShow : 1;
    unsigned int  isActCamo : 1;
    unsigned int  isHidden : 1;
    unsigned int  isInited : 1;          /* 0x37 */
} HCEX_OBJ;

/* --- snd audio system (large ws-engine systems: snd::BUFFER / snd::SYSTEM; touched here only
 * through Play/GetBuffer, kept opaque with free-function accessors per this corpus's boundary
 * convention). snd::BUFFER_PARAMS (32B) is fully modeled since it is built by value. --- */
typedef struct snd_BUFFER snd_BUFFER;   /* opaque — snd::BUFFER */
typedef struct snd_SYSTEM snd_SYSTEM;   /* opaque — snd::SYSTEM */

struct snd_NOTIFICATION_INFO; /* boundary */
typedef struct snd_BUFFER_PARAMS
{
    m3dV          pos;         /* 0x00 — DB m3dV; layout-identical to hcex_float3 */
    int           timeOffset;  /* 0x0C */
    bool          blocking;    /* 0x10 */
    unsigned char _pad11[3];   /* 0x11 db-verified padding */
    void        (*callback)(const struct snd_NOTIFICATION_INFO *, void *); /* 0x14 */
    void         *userData;    /* 0x18 */
    dsTSTRING<char>    dbgInfo;     /* 0x1C */
} snd_BUFFER_PARAMS;
typedef snd_BUFFER_PARAMS snd_BUFFER_PARAMS_FULL; /* legacy spelling (hcex_create_effect_impl) */

extern void        snd_BUFFER_Play(snd_BUFFER *self);                              /* snd::BUFFER::Play */
extern snd_BUFFER *snd_SYSTEM_GetBuffer(snd_SYSTEM *self, const char *id, snd_BUFFER_PARAMS *params); /* snd::SYSTEM::GetBuffer */

/* --- opaque container / scene / family handles --- */
typedef struct dsVECTOR_PAIR dsVECTOR_PAIR;   /* hcex_effect_names: dsVECTOR<dsPAIR<dsTSTRING_flat,dsTSTRING_flat>,8> */
typedef struct dsVECTOR_TSTR dsVECTOR_TSTR;   /* hcexUsedSfx / hcexMissedSfx: dsVECTOR<dsTSTRING_flat,8> */
typedef dsVECTOR<HCEX_EFFECT, 8> dsVECTOR_EFF;    /* hcexCreateEffects */
typedef dsVECTOR<FP_MODEL, 8>    dsVECTOR_FPM;    /* fpModels */
typedef dsPAIR<dsTSTRING_flat, dsTSTRING_flat> dsPAIR_TSTR; /* DB dsPAIR<dsTSTRING<char>,dsTSTRING<char>> (8B) */
typedef struct scnSCENE scnSCENE;
typedef struct dscDESC_FAMILY dscDESC_FAMILY;

/* InsertSorted duplicate policy */
#define INS_DUP_IGNORE 0

/* --- debug toggles: DB applied type dbgVAR_SIMPLE<bool,1> (value@0x0C via dbgVAR_IMPL base).
 * Deviation from the earlier flat hcex_debug_bool {int; uchar@4} model, which mis-placed
 * `value` — the disasm reads dbgVAR_SIMPLE<bool,1>.value (offset 12). --- */
extern dbgVAR_SIMPLE<bool, 1> dbg_hcex_create_effects;
extern dbgVAR_SIMPLE<bool, 1> dbg_hcexDumpUsedSfxOn;

/* --- globals --- */
extern int             hcex_last_killed_in_body_part;
extern int             hcex_coop_local_player_index;
extern int             hcex_skull_GruntBirthdayParty;
extern dsVECTOR_PAIR   hcex_effect_names;
extern dsVECTOR_TSTR   hcexUsedSfx;
extern dsVECTOR_TSTR   hcexMissedSfx;
extern dsVECTOR_EFF    hcexCreateEffects;
extern dsVECTOR_FPM    fpModels;
extern dscDESC_FAMILY  iaFamily;
extern scnSCENE       *gsScenePtr;
extern snd_SYSTEM     *snd_System;            /* snd::System */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

/* --- ws container methods (free-function form of the C++ thiscall) --- */
extern dsTSTRING_BUF_HEADER<char> *dsTSTRING_AllocBuffer(dsTSTRING_flat *s, int len, int maxLen);
extern void  dsTSTRING_UnsafeInit(dsTSTRING_flat *s, const char *src, int len, int flags);
extern void  dsTSTRING_assign(dsTSTRING_flat *dst, const dsTSTRING_flat *src);            /* operator= */
extern void  dsTSTRING_Insert(dsTSTRING_flat *s, int at, const char *src, int len);
extern void  dsTSTRING_InsertRepeat(dsTSTRING_flat *s, int at, int count);          /* Insert(at, n) overload */

extern int          dsVECTOR_PAIR_FindSorted(dsVECTOR_PAIR *v, const dsTSTRING_flat *key, dsCMP *cmp);
extern dsPAIR_TSTR *dsVECTOR_PAIR_index(dsVECTOR_PAIR *v, int i);
extern void         dsVECTOR_TSTR_InsertSorted(dsVECTOR_TSTR *v, const dsTSTRING_flat *s, dsCMP *cmp, int dup_policy);
extern HCEX_EFFECT *dsVECTOR_EFF_PushBack(dsVECTOR_EFF *v, const HCEX_EFFECT *src);
extern void         dsVECTOR_LOC_Resize(dsVECTOR<HCEX_EFFECT_LOC, 8> *v, int n);
extern HCEX_EFFECT_LOC *dsVECTOR_LOC_index(dsVECTOR<HCEX_EFFECT_LOC, 8> *v, int i);
extern FP_MODEL    *dsVECTOR_FPM_index(dsVECTOR_FPM *v, int i);

extern void  HCEX_EFFECT_ctor(HCEX_EFFECT *e);
extern void  HCEX_EFFECT_dtor(HCEX_EFFECT *e);

/* --- sml / snd / scene / family / ent --- */
extern void        sml_STATE_Set(sml_STATE *state, const dsTSTRING_flat *value);
extern snd_BUFFER *snd_BUFFER_params_ctor(snd_BUFFER_PARAMS *p);   /* snd::BUFFER_PARAMS::BUFFER_PARAMS */
extern void        snd_BUFFER_Release(snd_BUFFER *b);
extern void        dsSTRID_init(dsSTRID *s, const char *name, int flags);
extern animINST   *scnSCENE_FindInstName(scnSCENE *scene, const char *name);
extern int         dscDESC_FAMILY_FindBrand(dscDESC_FAMILY *family, const dsTSTRING_flat *brand);
extern int         entENTITY_GetNumSfx(entENTITY *ent);

/* --- hcex bridge helpers --- */
extern char  *hcex_conv_eff_name(const char *mdl_name, char *out, int max_len);
extern int    hcex_is_under_water(float x, float y, float z);
extern int    hcex_is_weapon(int id);
extern int    hcex_is_vehicle(int id);
extern HCEX_OBJ   *hcex_obj_get(int id);
extern entENTITY  *hcex_create_effect_impl(const dsTSTRING_flat *name, int obj_follow, const hcex_float3 *position);
extern void        hcex_prepare_locations(animINST *inst, const hcex_float3 *positions,
                       const hcex_float3 *vectors, const char **names, int npoints);
extern void        hcex_set_follow(entENTITY *ent, int obj_follow);

/* --- CRT / allocator --- */
extern void dlFree(void *ptr);
extern void *memcpy(void *dst, const void *src, unsigned int n);
extern int sprintf_0(char *string, const char *format, ...);
