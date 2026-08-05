#pragma once
/* Boundary types for hcex_obj_colorize. animINST / objOBJ are ws-engine scene-graph classes reached only to
 * read the instance's template name and root object and to recolor it — modeled to the touched fields only,
 * at DB offsets (animINST.nameTpl@0x30, animINST.pObj@0x148). SetColorMPHier is objOBJ::SetColorMPHier,
 * modeled as the free-function form of the thiscall. */

#include "HCEX_OBJ.h"   /* HCEX_OBJ; also forward-declares animINST, completed below */

typedef struct objOBJ objOBJ;

/* animINST — canonical full DB-verified layout (nameTpl@0x30, pObj@0x148); the former
 * per-consumer partial slice was consolidated. */
#include "../animINST.h"

extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern void        objOBJ_SetColorMPHier(objOBJ *self, unsigned int color);
extern HCEX_OBJ   *hcex_obj_get(int id);
