#pragma once
/* Boundary declarations for hcex_light_sync.c / hcex_light_sync_all.c.
 *
 * hcex_light_sync pushes one bridged Blam light's current radius/basis into its ws-engine entity each
 * frame (hiding/showing it, adjusting first-person-camera visibility, and rescaling any attached
 * entSFX_LIGHT effect). hcex_light_sync_all drives it over every entry of the sorted hcexLights vector.
 * The ws-engine ent/anim/ds container types are boundaries per the project's re-source conventions;
 * only the members these two functions actually touch are modeled. */

#include <stdint.h>
#include "hcex_light.h"          /* HCEX_LIGHT, hcex_light_pair, ds_WEAK_PTR */
#include "hcex_matr4x3.h"        /* hcex_matr4x3 */
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ent/entENTITY.h"
#include "../ws/anim/animINST.h"
#include "../ws/m3d/m3dMATR.h"

/* sorted dsVECTOR<dsPAIR<int,HCEX_LIGHT>,8> of every bridged Blam light, keyed by light id. */
#include "headers/real_matrix4x3.h"
extern dsVECTOR<hcex_light_pair, 8> hcexLights;

/* --- Blam-side light param resolver (already reversed: src/hcex/hcex_get_light_params.c) --- */
// DB-canonical 2nd param is hcex_matr4x3* (matches this file's hcex_matr4x3 callers); the
// hcex_get_light_params.c definition uses the layout-identical real_matrix4x3* internally. (2026-07-31)
extern void hcex_get_light_params(uint16_t lightId, hcex_matr4x3 *matr, float *radius, int *plr_idx);

/* --- Halo->ws instance-matrix builder (already reversed: src/hcex/hcex_make_inst_matr.c) --- */
extern void hcex_make_inst_matr(const hcex_matr4x3 *in, m3dMATR *out);

/* --- Blam-side first-person visibility check (already reversed: src/first_person_weapon_visible.c) --- */
extern int first_person_weapon_visible(int16_t plr_idx);

/* entENTITY virtual methods touched here (vtable dispatch in the binary; declared as free functions
 * per the project's compiler-generated-thiscall convention since entENTITY.h does not model a vtable). */
extern void entENTITY_implHide(entENTITY *self);
extern void entENTITY_implShow(entENTITY *self);
extern void entENTITY_ShowToCamera(entENTITY *self, int camMask);
extern void entENTITY_HideFromCamera(entENTITY *self, int camMask);

/* entSFX_LIGHT: a light-emitting sound/fx effect instance attached to an entity's sfx list. */
typedef struct entSFX entSFX;
typedef struct entSFX_LIGHT entSFX_LIGHT;
extern entSFX *entENTITY_GetSfx(entENTITY *self, int index);
extern int     entSFX_IsA_LIGHT(entSFX *sfx);                       /* entSFX::IsA<entSFX_LIGHT> */
extern void    entSFX_LIGHT_SetRadScale(entSFX_LIGHT *self, float scale);

extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent(void);

/* Raw bit masks used directly by hcex_light_sync; the DB surfaces no named enumerators for these,
 * so they're reproduced as-is (matching the disasm) rather than guessed at.
 *   entENTITY.stateEnt.val bit 12 (0x1000) — some ENT_ST "hidden" flag.
 *   animINST.state2 bits 0x80 / 0x100      — camera-visibility latches checked around the
 *                                            first-person HideFromCamera(0)/HideFromCamera(1) calls. */
#define ENT_ST_HIDDEN_BIT      0x1000u
#define ANIMINST_STATE2_BIT7   0x80u
#define ANIMINST_STATE2_BIT8   0x100u
