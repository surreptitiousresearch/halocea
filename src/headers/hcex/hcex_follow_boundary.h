#pragma once
/* Boundary types/functions for the HCEX object-follow bridge. The ws-engine math (m3dMATR),
 * container (dsVECTOR, ds::WEAK_PTR), and animation (animINST, entENTITY) templates are declared
 * here as opaque extern boundaries per the re-source conventions. */

#include "hcex_create_effect_boundary.h"   /* HCEX_OBJ, animINST, entENTITY */

/* 4x4 transform used by the ws animation/math layer (opaque to this layer). */
typedef struct m3dMATR m3dMATR;

/* HCEX's own row-major 4x3 transform shuttled across the Blam/ws boundary. */
#include "hcex_matr4x3.h" /* full hcex_matr4x3 layout (used by value in hcex_set_follow) */

/* ds::WEAK_PTR<entENTITY> — a refcounted weak handle wrapper; canonical templates
 * (ws/ds/WEAK_PTR.h), aliased to the boundary convention's flat names. */
#include "../ws/ds/WEAK_PTR.h"
typedef ds::WEAK_PTR_HANDLE<entENTITY> ds_WEAK_PTR_HANDLE;
typedef ds::WEAK_PTR<entENTITY> ds_WEAK_PTR;

/* HCEX_OBJ.followers / .followersOfs container ops (boundary). */
extern void dsVECTOR_WEAKPTR_PushBack(void *vec, ds_WEAK_PTR *item);
extern void dsVECTOR_MATR_PushBack(void *vec, m3dMATR *item);

/* ds::WEAK_PTR lifecycle + handle access (boundary). */
extern ds_WEAK_PTR_HANDLE *entENTITY_GetWeakHandle(entENTITY *e);
extern void ds_WEAK_PTR_dtor(ds_WEAK_PTR *p);
extern long osLockedIncrement(volatile long *value);

/* animation + math ops (boundary). */
extern void animINST_GetMatrL2W(animINST *inst, m3dMATR *out);
extern void m3dMATR_RemoveScale(m3dMATR *m);
extern void m3dMATR_Invert(const m3dMATR *m, m3dMATR *out);
extern void m3dMATR_Transform(m3dMATR *m, const m3dMATR *by, int mode);

/* HCEX bridge math helpers (reversed separately). */
extern "C" int  hcex_get_obj_matr(int objId, hcex_matr4x3 *obj_matr);
extern void hcex_make_inst_matr(const hcex_matr4x3 *src, m3dMATR *inst_matr);
extern HCEX_OBJ *hcex_obj_get(int id);
