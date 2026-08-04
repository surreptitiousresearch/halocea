#pragma once
/* Boundary declarations for hcex_on_play_sound.c.
 *
 * hcex_on_play_sound bridges a Blam "sound started" notification into the ws-engine lip-sync system: when a
 * dialog sound for the current level plays on a unit, it looks up (or lazily creates) that unit's
 * mdlLIP_SYNCER in the sorted hcexLipSyncList and starts it on the sound's leaf name. The ws template
 * containers (dsVECTOR / dsTSTRING_flat), the mdlLIP_SYNCER animation object and the placement operator new are
 * extern boundaries per the project's re-source conventions; only the touched fields are modeled. */

#include "haloENGINE_CONTROL.h"   /* haloENGINE_CONTROL, dsTSTRING_flat, dlFree, dsTSTRING_UnsafeInit (via hcex_ds_boundary.h) */
#include "HCEX_OBJ.h"             /* HCEX_OBJ (pInst -> animINST *) */
#include "../lipsync_pair.h"         /* lipsync_pair, mdlLIP_SYNCER (opaque) */
#include "../ds_vector_generic.h" /* ds_vector_generic */

typedef int dsCMP;

/* InsertSorted duplicate policy */
#define INS_DUP_IGNORE 0

/* --- globals --- */
extern haloENGINE_CONTROL *haloEngineCtrl;
extern ds_vector_generic   hcexLipSyncList;   /* dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8> */
extern const char          empty_string[];   /* "" */
extern const char          IGNORE_STRONG_ASSERT;

/* dsTSTRING<char> flat helpers / allocator (C++ view of haloENGINE_CONTROL.h does not pull the
 * C-only hcex_ds_boundary.h, so declare the two this TU uses directly). */
extern void dsTSTRING_UnsafeInit(dsTSTRING_flat *s, const char *src, int len, int flags);
extern "C" void dlFree(void *ptr);

/* --- ws / CRT / bridge helpers --- */
extern const char *hcex_tag_get_name(int sound_definition_index);
extern void        hcex_cine_on_play_sound(const char *name);
extern char       *strstr(const char *haystack, const char *needle);
extern char       *strrchr(const char *s, int c);
extern HCEX_OBJ   *hcex_obj_get(int id);
extern void        STRONG_ASSERT_DUMMY_Crash(void *a, const char *expr, const char *file, int line, const char *msg);

/* placement operator new(size, file, line) */
extern void *operator new(size_t size, const char *file, unsigned int line);

/* mdlLIP_SYNCER (ws-engine animation object; opaque) */
extern void mdlLIP_SYNCER_ctor(mdlLIP_SYNCER *self, animINST *inst);              /* mdlLIP_SYNCER::mdlLIP_SYNCER */
extern void mdlLIP_SYNCER_Start(mdlLIP_SYNCER *self, const dsTSTRING_flat *name, int flags);

/* dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8> ops (free-function thiscall form) */
extern int          dsVECTOR_lipsync_FindSorted(ds_vector_generic *v, const int *key, dsCMP *cmp);
extern int          dsVECTOR_lipsync_InsertSorted(ds_vector_generic *v, const lipsync_pair *entry, dsCMP *cmp, int dup);
extern lipsync_pair *dsVECTOR_lipsync_index(ds_vector_generic *v, int position);
