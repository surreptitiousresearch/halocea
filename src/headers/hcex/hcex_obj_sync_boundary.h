#pragma once
/* Boundary declarations for hcex_obj_sync.c.
 *
 * hcex_obj_sync pushes one tracked HCEX_OBJ's full per-frame render state into its ws-engine
 * animINST: camera-visibility latches (derived from the Blam "hidden from camera" mask), the
 * Halo->ws world transform (skipped, with a "way out of bounds" latch set, if the position is
 * absurdly large), every follower entity's offset transform, an active lip-sync update, and — unless
 * fully camera-hidden — the per-node local-transform/bounding-volume recompute and per-model-region
 * permutation visibility (SetStateProcYes/No) driven by HCEX_MODEL's node-index and region tables. */

#include "HCEX_OBJ.h"
#include "hcex_matr4x3.h"
#include "HCEX_MODEL.h"
#include "lipsync_pair.h"
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/WEAK_PTR.h"            /* ds::WEAK_PTR<entENTITY> */
#include "../ws/ent/entENTITY.h"
#include "../ws/anim/animINST.h"
#include "../ws/obj/objITER_TREE.h"
#include "../ws/obj/objOBJ.h"
#include "../ws/obj/objGEOM_UNSHARED.h"
#include "../ws/obj/objGEOM_SHARED.h"
#include "../ws/m3d/m3dBOX.h"
#include "../ws/m3d/m3dOBB.h"
#include "../ws/m3d/m3dMATR.h"
#include "../ws/wb/dbgVAR_boundary.h"     /* dbgVAR_SIMPLE<bool,1> shape for dbg_hcex_off_bbox_calc */

/* dsVECTOR<dsPAIR<int,mdlLIP_SYNCER*>,8> — see hcex_on_play_sound_boundary.h for the fuller version;
 * only FindSorted/index are needed here. */
typedef struct ds_vector_generic ds_vector_generic;
extern ds_vector_generic hcexLipSyncList;
#include "../ws/ds/dsCMP.h"   /* canonical dsCMP (stateless empty struct) — a local `typedef int dsCMP` is a C2371/ODR conflict with it */
extern int           dsVECTOR_lipsync_FindSorted(ds_vector_generic *v, const int *key, dsCMP *cmp);
extern lipsync_pair *dsVECTOR_lipsync_index(ds_vector_generic *v, int position);
extern void          mdlLIP_SYNCER_ApplyLipSync(mdlLIP_SYNCER *self);

/* hcexModels — DB dsVECTOR<dsPAIR<unsigned long,HCEX_MODEL>,8>, via the canonical templates. */
#include "../ws/ds/dsPAIR.h"
typedef dsPAIR<unsigned long, HCEX_MODEL> hcex_model_pair;
typedef dsVECTOR<dsPAIR<unsigned long, HCEX_MODEL>, 8> dsVECTOR_HCEX_MODEL_8;
extern dsVECTOR_HCEX_MODEL_8 hcexModels;

/* --- already-reversed hcex bridge helpers this function drives --- */
extern "C" unsigned int hcex_object_hidden_cam_mask(int id);     /* hcex_object_hidden_cam_mask.c */
extern "C" int          hcex_get_obj_matr(int objId, hcex_matr4x3 *obj_matr);   /* hcex_get_obj_matr.c */
extern void         hcex_make_inst_matr(const hcex_matr4x3 *in, m3dMATR *out); /* hcex_make_inst_matr.c */
extern "C" int          hcex_get_obj_node_matrices(int objId, hcex_matr4x3 *out_node_matrices,
                        unsigned char *out_permutations);      /* hcex_get_obj_node_matrices.c */

/* --- ws-engine obj tree LTM helper (not one of this batch's targets) --- */
extern void objCalcNodeLTM(struct objOBJ *obj);

/* --- already-reversed sibling of hcex_make_inst_matr, used for per-node matrices --- */
extern void hcex_make_matr(const hcex_matr4x3 *in, m3dMATR *out);   /* hcex_make_matr.c */

/* --- m3d finiteness assertion (see ws/m3d/m3d_boundary.h) --- */
extern int _m3dCheckValid(const m3dV *v);

#include <math.h>   /* fabsf */

/* --- entENTITY virtual "impl*" camera-visibility calls at raw vtable slots 392/396 (98/99). The
 * decompiler shows these as bare `(*(void(**)(void))(vt+392))()` calls with no visible argument
 * (distinct call sites from the 1-arg ShowToCamera/HideFromCamera used by hcex_light_sync — same
 * vtable region, different slots); reproduced as raw vtable dispatch rather than guessing a name. */
static inline void entENTITY_vtblCall98(struct entENTITY *self)
{
    void (**vtbl)(struct entENTITY *) = *(void (***)(struct entENTITY *))self;
    vtbl[98](self);
}
static inline void entENTITY_vtblCall99(struct entENTITY *self)
{
    void (**vtbl)(struct entENTITY *) = *(void (***)(struct entENTITY *))self;
    vtbl[99](self);
}

/* dbg_hcex_off_bbox_calc — debug toggle to skip the per-frame bounding-volume recompute. */
extern "C" dbgVAR_SIMPLE<bool, 1> dbg_hcex_off_bbox_calc;

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
