#pragma once
/* Boundary declarations for hcex_change_render_mode.cpp (@0x823DEE38).
 *
 * The function commits a pending render-mode switch (hcex_new_render_mode -> hcex_cur_render_mode):
 * reconfigures gamma on the d3dDRIVER, flushes the render-node/light/shadow state, GPU-fences,
 * drops the streamed-texture worklist, and (when switching back to mode 0) walks every managed
 * texture releasing its streaming resource and re-precaches. All touched types are ws-engine / D3D
 * boundaries; only the touched fields/methods are modeled at their DB-verified offsets. */

#include "../d3d_driver.h"                  /* d3dDRIVER (+ vidDRIVER base), d3dDriver global */
#include "../ws/txm/txmTEXTURE.h"           /* txmTEXTURE (state@0x78, strmResource@0x90, UnloadResource) */
#include "../ws/strm/strmRESOURCE_TEXTURE.h"/* strmRESOURCE_TEXTURE (minMip@56, removingMip@76) */
#include "../ws/strm/strmVID_MEM_MANAGER.h" /* strmVID_MEM_MANAGER (Sync, ReserveAllMemory) */
#include "../ws/strm/strmRES_TEX_MANAGER.h" /* strmRES_TEX_MANAGER (Clear, ::mng) */
#include "../ws/rend/rendLGT_MNG.h"         /* rendLGT_MNG (ClearActiveLights, PushLights..., rendLgtMng) */
#include "../ws/ds/dsVECTOR.h"              /* dsVECTOR<txmTEXTURE*,8> — iterator element store */

/* --- ws texture manager --- */
/* Canonical full DB layout (types_members txmMANAGER, size 928) plus its ITERATOR/Begin(). This
 * header used to carry a second, member-less `typedef struct txmMANAGER` next to the canonical
 * one — a duplicate definition of a type that already owns a header, and the reason db_verify
 * reported all 25 DB members missing. Include the real one instead; ITERATOR and Begin() moved
 * there (a nested type cannot live outside its class). `txmManager` is declared there too. */
#include "../ws/txm/txmMANAGER.h"

/* --- gs shadow-map subsystem (opaque; only ClearLights is called) --- */
/* Deliberately opaque: the DB type is 51,160 bytes over 45 members (nCurLight, rendSMTargeters[50],
 * smParams[8][4], …), none of which this path reads. It is reached only through the gsShadowMap
 * pointer below — never constructed, sized or indexed here — and no canonical header models it
 * (gsRENDER_SYSTEM.h forward-declares it for the same reason). */
typedef struct gsSHADOW_MAP_SYSTEM {
    // Body lives in the gs subsystem — boundary (called from hcex_change_render_mode).
    void ClearLights();
} gsSHADOW_MAP_SYSTEM;

/* --- globals (DB symbol names) --- */
extern int         hcex_cur_render_mode;   /* ?hcex_cur_render_mode / current committed mode */
extern int         hcex_new_render_mode;   /* pending mode requested by the s3d/screen-mode path */
extern int         hcex_tex_block;         /* force blocking texture stream during re-precache */
extern int         vidUseGammaCorrection;  /* d3d_ensure_texture_boundary: gamma-correction toggle */
extern "C" unsigned char debug_no_drawing;     /* blam debug: suppress drawing */
extern vidDRIVER  *vidDriver;              /* render-thread driver (thread-ownership hooks) */
extern gsSHADOW_MAP_SYSTEM *gsShadowMap;   /* ?gsShadowMap@@3PAVgsSHADOW_MAP_SYSTEM@@A */
extern strmVID_MEM_MANAGER strmMemMng;     /* ?strmMemMng@@3VstrmVID_MEM_MANAGER@@A */

/* --- free-function boundaries --- */
extern void osPIXBeginEvent(const char *label);
extern void osPIXEndEvent(void);
extern void hcex_render_targets_set_gamma(int on);
extern "C" void hcex_precache_resources(void);
extern void rnsMNG_ClearAndSwap(void);

/* D3D fence primitives — D3D SDK boundary (stay extern). */
extern "C" unsigned int D3DDevice_InsertFence(D3DDevice *device);
extern "C" void         D3DDevice_BlockOnFence(unsigned int fence);
