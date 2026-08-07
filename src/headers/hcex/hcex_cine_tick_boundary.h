#pragma once
/* Boundary declarations shared by the HCEX cinematic-tick bridge functions: hcex_cine_init,
 * hcex_cine_update, hcex_cine_update_anim, hcex_cine_term_level, hcex_cine_process_start,
 * hcex_unhide_actors, hcex_hide_obj_follow_hier. ws-engine C++ containers/classes are modeled as
 * flattened C boundaries per the project convention: only the fields the bridge touches are declared,
 * at their database-verified offsets, and C++ methods are declared as free functions taking the object
 * as their first parameter. */

#include <stdint.h>
#include "HCEX_OBJ.h"             /* HCEX_OBJ, HCEX_OBJ_FLAG_IS_INITED/IS_HIDDEN */
#include "../animINST.h"          /* animINST (pInst, pTpl, state) */
#include "hcex_ds_boundary.h"     /* dsTSTRING_flat, dsTSTRING_BUF_HEADER, dsSTRID, dlFree */
#include "hcex_dbgvar_boundary.h" /* dbgVAR_bool toggles */
#include "DBG_CINE_INFO.h"        /* DBG_CINE_INFO, DBG_CINE_ACTOR_INFO, mapActors ops */

#include "../ws/ap/apSTATE_T.h" /* apSTATE_T<unsigned char> (hcexCINE_INFO::mask) */
#include "../ws/ds/dsVECTOR.h"  /* real dsVECTOR<T,8> template (shared with the canonical ws headers) */

/* The named helper typedefs below are aliases to dsVECTOR<T,8> instantiations so the extern
 * signatures / .c consumers keep their spellings; each resolves to a real ws container type. */
typedef dsVECTOR<void *, 8> dsVECTOR_hdr;      /* generic header view */
typedef dsVECTOR<int, 8>    dsVECTOR_int_hdr;  /* cineHiddenHcexObjKeys: dsVECTOR<int,8> */

/* --- cinematic-object name -> Blam id table: dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8> --- */
#include "hcex_cine_pair.h" /* hcex_cine_pair */
typedef dsVECTOR<hcex_cine_pair, 8> hcex_cine_pair_vec;

/* --- per-mask cine-actor binding (hcexCINE_INFO, DB size 8) and the owning list-of-lists (listCine) --- */
struct gsANITEC_ACTOR; /* boundary — cinematic actor runtime record */

#include "hcexCINE_INFO.h" /* hcexCINE_INFO — canonical single definition (actor@0, mask@4) */

typedef dsVECTOR<hcexCINE_INFO, 8> dsVECTOR_hcexCINE_INFO;

#include "../ws/ds/dsPAIR.h"
/* dsPAIR<dsTSTRING<char>, dsVECTOR<hcexCINE_INFO,8>> — key = script name, val = per-difficulty
 * actor/mask table. (dsTSTRING_flat flat here is layout-compatible with dsTSTRING<char>.) */
typedef dsPAIR<dsTSTRING_flat, dsVECTOR_hcexCINE_INFO> hcex_cine_list_pair;

typedef dsVECTOR<hcex_cine_list_pair, 8> dsVECTOR_cine_list;

/* --- entENTITY-owning list on the Anitec cinematic system (gsANITEC_SYS), boundary --- */
typedef struct entENTITY entENTITY; /* boundary — ws entity base */

typedef dsVECTOR<entENTITY *, 8> dsVECTOR_ent_hdr;

/* gsANITEC_SYS — canonical full DB-verified layout (ownedActors @0x120); the former per-consumer
 * partial slice was consolidated. */
#include "../ws/gs/gsANITEC_SYS.h"

/* --- globals --- */
extern gsANITEC_SYS       *gsSysAnitec;
extern hcex_cine_pair_vec   hcexObjectsCine;         /* dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8> */
extern dsVECTOR_hdr         cineHiddenActorsTplNames; /* dsVECTOR<dsTSTRING<char>,8> */
extern dsVECTOR_int_hdr     cineHiddenHcexObjKeys;    /* dsVECTOR<int,8> */
extern dsVECTOR_cine_list   listCine;                 /* dsVECTOR<dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>>,8> */
extern DBG_CINE_INFO        _dbgCineInfo;
extern int                  gsAnitecElapsedTicsExternal;
extern float                gsElapsedTime;
extern "C" float                totaltime;
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */

#include "../ws/ds/dsCMP.h"   /* canonical dsCMP (stateless empty struct) — a local `typedef int dsCMP` is a C2371/ODR conflict with it */
#define INS_DUP_IGNORE 1   /* dsVECTOR<...>::INS_SORT::INS_DUP_IGNORE */

/* --- ws / CRT / bridge helpers (free-function form of the C++ thiscall) --- */
extern HCEX_OBJ   *hcex_obj_get(int id);
extern HCEX_OBJ   *hcex_obj_get_by_idx(int idx);
extern "C" const char *hcex_obj_get_name(int id);
extern "C" void hcex_obj_get_child_and_next(int id, int *idChild, int *idNext);
extern "C" int hcex_obj_get_anim_info(int id, int *frameCur, int *frameCount, char **ptrName);
extern "C" int         hcex_is_cine_mode(void);
extern "C" int hcex_get_elapsed_tics(void);
extern int         gsMsgIsMode(unsigned int mode);
extern void        hcex_hide_obj_follow_hier(int id, int isHide, int isHideNext);
extern int         _apForceLog(const char *path, const char *fmt, ...);
extern "C" int16_t game_difficulty_level_get(void);

/* dsSTRID / dsTSTRING_flat template ops beyond hcex_ds_boundary.h's base set */
extern dsTSTRING_flat *dsTSTRING_concat(dsTSTRING_flat *out, const dsTSTRING_flat *a, const dsTSTRING_flat *b);      /* operator+ */
extern dsTSTRING_flat *dsTSTRING_concat_cstr(dsTSTRING_flat *out, const dsTSTRING_flat *a, const char *b);      /* operator+ */
extern void        dsTSTRING_assign(dsTSTRING_flat *dst, const dsTSTRING_flat *src);                       /* operator= */
extern void        dsTSTRING_Insert(dsTSTRING_flat *s, int at, const char *src, int len);

/* dsVECTOR<dsTSTRING<char>,8>::Clear on cineHiddenActorsTplNames */
extern void dsVECTOR_tstring_Clear(dsVECTOR_hdr *v);

/* dsVECTOR<dsPAIR<dsTSTRING<char>,int>,8> ops on hcexObjectsCine */
extern int  dsVECTOR_cine_pair_FindSorted(hcex_cine_pair_vec *v, const dsTSTRING_flat *key, dsCMP *cmp);
extern int  dsVECTOR_cine_pair_InsertSorted(hcex_cine_pair_vec *v, const hcex_cine_pair *entry, dsCMP *cmp, int dup);
extern hcex_cine_pair *dsVECTOR_cine_pair_index(hcex_cine_pair_vec *v, int position);

/* dsVECTOR<dsPAIR<dsTSTRING<char>,dsVECTOR<hcexCINE_INFO,8>>,8> ops on listCine */
extern void                  dsVECTOR_cine_list_Clear(dsVECTOR_cine_list *v);
extern int                   dsVECTOR_cine_list_FindSorted(dsVECTOR_cine_list *v, const dsTSTRING_flat *key, dsCMP *cmp);
extern hcex_cine_list_pair  *dsVECTOR_cine_list_index(dsVECTOR_cine_list *v, int position);

/* dsVECTOR<hcexCINE_INFO,8> ops on a listCine entry's per-difficulty table */
extern hcexCINE_INFO *dsVECTOR_hcexCINE_INFO_index(dsVECTOR_hcexCINE_INFO *v, int position);
extern hcexCINE_INFO *dsVECTOR_hcexCINE_INFO_Back(dsVECTOR_hcexCINE_INFO *v);

/* dsVECTOR<int,8>::InsertSorted on cineHiddenHcexObjKeys */
extern int dsVECTOR_int_InsertSorted(dsVECTOR_int_hdr *v, const int *val, dsCMP *cmp, int dup);

/* dsVECTOR<entENTITY*,8> indexed access on gsANITEC_SYS::ownedActors */
extern entENTITY **dsVECTOR_ent_index(dsVECTOR_ent_hdr *v, int position);
/* entENTITY's virtual GetName()-equivalent (vtable slot used at the ownedActors call site) */
extern void entENTITY_GetName(entENTITY *self, dsTSTRING_flat *out);

/* dsVECTOR<T,8>::operator[] inlines a STRONG_ASSERT2(idx>=0 && idx<nElem) in ds_vector.hpp:567;
 * reproduced here as the flattened helper the decompiler shows inlined at the cineHiddenHcexObjKeys
 * scan in hcex_unhide_actors. */
extern void STRONG_ASSERT2_asserd_int(const char *expr, const char *file, int line,
                                       const char *name1, const int *val1,
                                       const char *name2, const int *val2);

/* gsANITEC_ACTOR::StartCine() — begin playing the cinematic clip bound to this actor. */
extern void gsANITEC_ACTOR_StartCine(struct gsANITEC_ACTOR *self);
