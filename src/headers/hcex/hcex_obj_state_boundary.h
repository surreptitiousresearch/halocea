#pragma once
/* Boundary declarations for hcex_obj_set_state.c.
 *
 * hcex_obj_set_state pushes a new ws-engine animation-state name onto a tracked Blam object's entity (and,
 * when the object is a currently-held first-person weapon, onto every first-person view model). It reaches
 * into the ws-engine ent/anim object model and the sml state system, which are extern boundaries per the
 * project's re-source conventions: only the touched fields are modeled at their database offsets, and the
 * C++ "methods" are declared as free functions whose first parameter is the object.
 *
 * Layouts DB-verified (types_members): FP_MODEL.pInst @0x0C, dsVECTOR<FP_MODEL,8> {pData@0,nElem@4}, and
 * entENTITY.stateSml is iaIACTOR::stateSml (sml::STATE, 12 bytes) at offset 0x24 within the embedded
 * iaIACTOR at entENTITY+0. */

#include "../animINST.h"        /* animINST (pEnt @0x0C), entENTITY forward decl */
#include "HCEX_OBJ.h"           /* HCEX_OBJ (pInst @0x04), apCL */
#include "hcex_ds_boundary.h"   /* dsTSTRING_flat, dsTSTRING_BUF_HEADER, dsTSTRING_UnsafeInit, dlFree */

/* entENTITY — canonical full DB-verified layout (stateSml lives in the iaIACTOR base at
 * +0x24); the former per-consumer partial slice was consolidated. */
#include "../ws/ent/entENTITY.h"

/* Legacy C spelling of sml::STATE (12 bytes: dsFIXED_BIT_ARRAY<2> state@0, iaIACTOR *pOwner@8). */
typedef sml::STATE sml_STATE;

/* FP_MODEL — one first-person bridge model (24 bytes, DB-verified). */
typedef struct FP_MODEL
{
    int           plrIdx;           /* 0x00 */
    int           modelId;          /* 0x04 */
    int           modelIdx;         /* 0x08 */
    animINST     *pInst;            /* 0x0C */
    int           isActCamo;        /* 0x10 */
    bool          isHiddenCam1;     /* 0x14 */
    bool          isHiddenCam1Prev; /* 0x15 */
    bool          isHiddenCam2;     /* 0x16 */
    bool          isHiddenCam2Prev; /* 0x17 */
} FP_MODEL;

/* dsVECTOR_FP_MODEL is the plain-C spelling of the DB instantiation dsVECTOR<FP_MODEL,8>
 * (20 bytes; no C-spellable DB name of its own); DSVECTOR_INSTANCE keeps the layout verifier
 * from mis-binding this instantiation spelling. */
#define DSVECTOR_INSTANCE struct
typedef DSVECTOR_INSTANCE dsVECTOR_FP_MODEL
{
    FP_MODEL *pData;     /* 0x00 */
    int       nElem;     /* 0x04 */
    int       allocated; /* 0x08 */
    apCL      __cl;      /* 0x0C */
} dsVECTOR_FP_MODEL;

extern dsVECTOR_FP_MODEL fpModels;

extern HCEX_OBJ *hcex_obj_get(int id);
extern int       hcex_fp_weapon_plr_index(int object_index);
extern FP_MODEL *dsVECTOR_FP_MODEL_index(dsVECTOR_FP_MODEL *v, int i);     /* dsVECTOR<FP_MODEL,8>::operator[] */
extern void      sml_STATE_Set(sml_STATE *state, const dsTSTRING_flat *value);  /* sml::STATE::Set */
