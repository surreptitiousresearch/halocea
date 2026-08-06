#pragma once
/* Boundary declarations for hcex_set_cam.c.
 *
 * hcex_set_cam pushes one render window's camera into the ws-engine camera (camCAMERA) — or, when the
 * "off cam" latch is set, reads the camera back into the halo_cam_* globals for the Blam side. The ws camera,
 * the m3d math library, the Anitec cinematic subsystem, and the debug-variable system are all ws-engine
 * boundaries; only the fields the bridge touches are modeled, at their database offsets. m3dV/m3dMATR come
 * from hcex_prepare_locations_boundary.h. */

#include "hcex_prepare_locations_boundary.h"   /* m3dV, m3dMATR, hcex_float3 */

/* halo world unit -> ws unit scale (1 world unit = 3.0480001). */
#define HCEX_WORLD_SCALE 3.0480001f
/* ws unit -> halo world unit scale (0.32808399 = 1/3.0480001), used when reading the camera back. */
#define WS_TO_HALO_SCALE 0.32808399f

/* --- ws camera and Anitec cinematic system: canonical DB-verified full layouts (the former
 * per-consumer partial slices were consolidated) --- */
#include "../ws/cam/camCAMERA.h"      /* camCAMERA (matrC2W@0, ...) */
#include "../ws/gs/gsANITEC_SYS.h"    /* gsANITEC_SYS (pObjCam@0xEC, ...) */
#include "../ws/wb/dbgVAR_boundary.h" /* dbgVAR / dbgVAR_IMPL / dbgVAR_SIMPLE templates */

/* --- debug variables (dbgVAR base is 12 bytes; the value follows at 0x0C) --- */
/* dbgVAR_TRIGGER — DB: a single anonymous dbgVAR_SIMPLE<bool,5> base at offset 0 (16 bytes);
 * the bool payload lives at base offset 0x0C (dbgVAR_IMPL layer). */
typedef struct dbgVAR_TRIGGER
{
    dbgVAR_SIMPLE<bool,5> base;  /* 0x00 — value@0x0C */
} dbgVAR_TRIGGER;

/* dbgVAR_float has a single definition in hcex_dbgvar_boundary.h (dbgVAR_SIMPLE<float,3> spelling);
 * include it rather than redefining locally, so TUs pulling both headers don't clash (C2371). */
#include "hcex_dbgvar_boundary.h"

/* --- globals --- */
extern "C" dbgVAR_TRIGGER dbg_hcex_DumpModels;
extern "C" dbgVAR_float   dbg_hcex_CustomFov;
extern "C" unsigned char  dumpModels;
extern unsigned char  hcex_off_cam;
extern "C" int            halo_cam_sync_back;
extern "C" hcex_float3    halo_cam_pos;
extern "C" hcex_float3    halo_cam_up;
extern "C" hcex_float3    halo_cam_forv;
extern const m3dV     hcexWorldOffset;
extern "C" float          offsetZ;
extern gsANITEC_SYS  *gsSysAnitec;

/* --- ws math / camera / anitec methods (boundary) --- */
extern int  gsCameraCount(void);
extern camCAMERA *gsCameraGet(int idx);
extern int  _m3dCheckValid(m3dV *v);
extern void m3dCrossProduct(const m3dV *a, const m3dV *b, m3dV *out);
extern void m3dV_add_assign(m3dV *self, const m3dV *v);                       /* m3dV::operator+= */
extern void _m3dLineCombine(const m3dV *a, const m3dV *b, float kb, m3dV *out);/* out = a + kb*b */
extern void camCAMERA_SetParameters(camCAMERA *self, const m3dV *org, const m3dV *up,
        const m3dV *right, const m3dV *forward);
extern void camCAMERA_SetAngleHor(camCAMERA *self, float angleHor);

/* --- hcex bridge helpers --- */
extern "C" void hcex_dump_all_models(void);
extern void hcex_cine_set_cam(camCAMERA *camera);
