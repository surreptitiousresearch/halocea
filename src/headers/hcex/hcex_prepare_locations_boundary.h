#pragma once
/* Boundary types/functions for hcex_prepare_locations. The ws-engine math (m3dMATR/m3dV/m4dV) and
 * scene object (objOBJ/animINST) types are the canonical DB-verified ws headers (previously partial
 * local duplicates; consolidated). */

#include "hcex_float3.h"            /* hcex_float3 — single definition, shared with hcex_matr4x3.h */
#include "../ws/m3d/m3dV.h"
#include "../ws/m3d/m4dV.h"
#include "../ws/m3d/m3dMATR.h"
#include "../ws/anim/objOBJ_boundary.h" /* full DB-verified objOBJ graph */

typedef struct animINST animINST;    /* opaque; only pObj@0x148 is touched, via accessor below */

extern objOBJ *animINST_get_pObj(animINST *inst);   /* inst->pObj (0x148) */

extern objOBJ *objFindName(objOBJ *root, const char *name);
extern void    m3dMATR_Invert(const m3dMATR *src, m3dMATR *out);
extern void    m3dMATR_MakeLCS2WCS_VZ(m3dMATR *out, const m3dV *origin, const m3dV *forward);
extern char   *hcex_conv_name(char *name, int max_len);
