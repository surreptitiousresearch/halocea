#pragma once
/* Boundary declarations for hcex_obj_sync_game_state.c (both the 0-arg driver and the 1-arg
 * per-object worker).
 *
 * hcex_obj_sync_game_state(HCEX_OBJ*) pushes every Blam "game state" input the ws-engine's HALO_OBJ
 * property (propHALO_OBJ) cares about into that property, for one tracked object, once per frame:
 *   - the four SSL script "function in" channels (propHALO_OBJ::funcIn[0..3]) and "function out"
 *     channels (funcOut[0..3]) — each fires an sslOBJ_REF::HandleEvent SSL notification (using the
 *     event ids configured on the object's propHALO_OBJ_DESC) only when the sampled value changed;
 *   - live entSFX_COLOR intensity (from function channel 1);
 *   - shield-effect transparency (fades the "shield" sub-object in/out via SetTransparency);
 *   - active-camouflage material swap;
 *   - machine locked/unlocked (fires the well-known hcexMachineLock/hcexMachineUnlock SSL script
 *     functions) and machine powered on/off (drives the entity's sml state machine);
 *   - the four generic FUNC_A..D sml states (also driven by function channels 0..3);
 *   - "machine position" transparency (same SetTransparency dance as the shield effect).
 * Which of the above are active for this object is a per-descriptor bitmask (propHALO_OBJ_DESC's
 * `objUse` field). None of this fires unless the entity actually carries a propHALO_OBJ property
 * (found either directly, when the propContainer's own descriptor type IS propHALO_OBJ_DESC, or by
 * searching its properties/components lists when the descriptor is a propCONTAINER_DESC).
 *
 * The prop/ssl/sml subsystems are ws-engine boundaries; only the shape actually touched is modeled
 * (propHALO_OBJ_DESC/propCONTAINER_DESC/propBASE/propCONTAINER/entENTITY/sslOBJ_REF/sml::STATE are
 * all already fully-typed elsewhere in this corpus and reused here). */

#include <stdint.h>
#include "HCEX_OBJ.h"
#include "hcex_obj_state_boundary.h"        /* sml_STATE, entENTITY (stateSml @0x24) */
#include "hcex_obj_customize_boundary.h"    /* dsTYPE_ID_IsDerivedFrom, propCONTAINER_DESC_TYPE_ID */
#include "hcex_param_list_boundary.h"       /* dsDATA, dsDATA_TYPE, dsDATA_TYPE_Destroy */

/* Canonical ws-engine prop types (bound to the DB, mirroring the dsTSTRING_flat refactor in
 * hcex_ds_boundary.h): the former propCONTAINER_flat / propHALO_OBJ_flat / propHALO_OBJ_DESC_flat /
 * entENTITY_prop_view byte-array views are now typedef aliases onto the real DB-verified C++ types.
 * Member access in the consumer goes through their real fields (propContainer via the iaIACTOR base,
 * propBASE::spDesc as a dsSMART_PTR whose ::pointee is the raw pointer, objUse as apSTATE_T::state). */
#include "../ws/prop/propCONTAINER.h"
#include "../ws/prop/propHALO_OBJ.h"
#include "../ws/prop/propHALO_OBJ_DESC.h"

/* entENTITY already carries propContainer at its DB offset (iaIACTOR::propContainer @0x60), so the
 * former wide "prop view" is just the canonical entity. */
typedef entENTITY         entENTITY_prop_view;
typedef propHALO_OBJ      propHALO_OBJ_flat;      /* propENT base@0 + timeTerm@0x28 + funcOut[4]@0x2C + funcIn[4]@0x3C */
typedef propHALO_OBJ_DESC propHALO_OBJ_DESC_flat; /* sslEventId_OnChange{In,Out}{A..D}@0x38.., objUse@0x58 (apSTATE_T) */

extern const dsTYPE_ID propHALO_OBJ_DESC_TYPE_ID;   /* propHALO_OBJ_DESC::TYPE_ID */

/* --- sslOBJ_REF methods (canonical sslOBJ_REF pulled in via propBASE.h) --- */
extern void sslOBJ_REF_copy_ctor(sslOBJ_REF *self, const sslOBJ_REF *src);
extern void sslOBJ_REF_dtor(sslOBJ_REF *self);
/* ?HandleEvent@sslOBJ_REF@@QAA?AVsslERROR@@HABVdsDATA@@0@Z — fire SSL event `eventId` on this
 * object with (newVal, oldVal); sret return (sslERROR) discarded by every caller here. */
extern void sslOBJ_REF_HandleEvent(sslOBJ_REF *self, int eventId, const dsDATA *newVal, const dsDATA *oldVal);
/* ssl script-function call (used for the machine lock/unlock notifications, dsSTRID-keyed). */
extern void sslOBJ_REF_CallFunc(sslOBJ_REF *self, const dsSTRID *funcName, int nArgs, const dsDATA **args);
extern dsSTRID hcexMachineLock;    /* "hcexMachineLock" */
extern dsSTRID hcexMachineUnlock;  /* "hcexMachineUnlock" */

extern void dsDATA_SetValue_float(dsDATA *data, const float *value);

/* --- prop object-model lookups --- */
extern propBASE      *propBASE_GetProperty_HALO_OBJ(propBASE *self);   /* propBASE::GetProperty<propHALO_OBJ> */

/* entENTITY virtual "impl*" methods (vtable dispatch in the binary; free-function form per this
 * corpus's convention — see hcex_light_sync_boundary.h's entENTITY_implHide/implShow). */
extern void entENTITY_implSetMaterial(entENTITY *self, const dsTSTRING_flat *material, const dsTSTRING_flat *subMaterial);

/* --- entSFX / entSFX_COLOR (color-tint sound/fx effect) --- */
typedef struct entSFX entSFX;
typedef struct entSFX_COLOR entSFX_COLOR;
extern int    entENTITY_GetNumSfx(entENTITY *self);
extern entSFX *entENTITY_GetSfx(entENTITY *self, int index);
extern int    entSFX_IsA_COLOR(entSFX *sfx);                              /* entSFX::IsA<entSFX_COLOR> */
extern void   entSFX_COLOR_implSetIntensity(entSFX_COLOR *self, float intensity);

/* animINST::pObj (DB offset 0x148) — accessed via a free function rather than extending the flat
 * `animINST` struct already declared (with only `pEnt`, offset 0x0C) by hcex_obj_customize_boundary.h,
 * to avoid a conflicting redefinition across the two headers. */
extern struct objOBJ *animINST_GetObjRoot(animINST *inst);

/* --- obj tree helpers (already reversed elsewhere in this corpus) --- */
extern struct objOBJ *objFindName(struct objOBJ *root, const char *name);
extern void objOBJ_SetStateProcYes(struct objOBJ *self, int stateProc, void *cbCond);
extern void objOBJ_SetStateProcNo(struct objOBJ *self, int stateProc, void *cbCond);
/* deviation: decompiled as a 64-bit argument (`__int64`); every call site here only ever passes
 * (value*255.0) cast down, so modeled as a plain int alpha [0,255] — the real width isn't load-bearing
 * for any caller in this batch. */
extern void objOBJ_SetTransparency(struct objOBJ *self, int alpha);

/* --- Blam-side / hcex-bridge queries (already reversed) --- */
extern float hcex_obj_get_function_in(int object_index, int function_index);   /* hcex_obj_get_function_in.c */
extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern float hcex_shield_effect_on(int id);          /* hcex_shield_effect_on.c */
extern int hcex_obj_is_act_camo(int id);            /* hcex_obj_is_act_camo.c */
extern int hcex_machine_is_locked(int id);          /* hcex_machine_is_locked.c */
extern int hcex_machine_is_power_on(int id);        /* hcex_machine_is_power_on.c */
extern float hcex_machine_position(int id);           /* hcex_machine_position.c */

extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent(void);

extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */

/* --- dsVECTOR<HCEX_OBJ,8> (hcexObjects) — canonical DB instantiation --- */
#include "../ws/ds/dsVECTOR.h"
typedef dsVECTOR<HCEX_OBJ, 8> dsVECTOR_HCEX_OBJ_8;
extern dsVECTOR_HCEX_OBJ_8 hcexObjects;
