#pragma once
/* Boundary declarations for hcex_render_hud.c.
 *
 * hcex_render_hud is the HCEX bridge's per-frame HUD/scene draw hook: it updates the stereo-3D crosshair
 * depth offset, refreshes the HALO render targets, and (holding hcexHaloLogic) drives one or two Blam
 * camera passes plus a final shared pass through the legacy Blam draw pipeline (prepare, screen flash,
 * interface/HUD, cinematic overlay, timer, terminal, FX-texture release) before restoring D3D render
 * state. Every driver/manager class it touches (vidDRIVER[_INTERFACE], txmMANAGER, gsRENDER_SYSTEM,
 * d3dDRIVER, osLOCK) is a large ws-engine boundary; only the fields/methods this function reads are
 * modeled here, at DB-verified offsets. Virtual dispatch is declared as free functions taking the
 * object as the first parameter (the corpus convention). */

#include <stdint.h>
#include "hcex_dbgvar_boundary.h"   /* dbgVAR_bool, dbgVAR_float */

/* --- ws-engine driver / manager classes: canonical full DB-verified layouts (the former
 * per-consumer partial slices were consolidated) --- */
typedef struct d3dDRIVER d3dDRIVER;
struct osLOCK; /* opaque here — only passed by pointer to osLOCK__Lock/osLOCK_Unlock (see hcex_wait_jobs.c) */

#include "../vidMODE.h"
#include "../vidDRIVER.h"
#include "../ws/txm/txmMANAGER.h"
#include "../ws/gs/gsRENDER_SYSTEM.h"

/* --- globals --- */
extern vidDRIVER       *vidDriver;
extern txmMANAGER      *txmManager;
extern gsRENDER_SYSTEM *gsSysRender;
extern d3dDRIVER       *d3dDriver;

extern struct osLOCK hcexHaloLogic;

extern "C" float crosshairOffsetX;
extern float g_Stereo3DUIConstants[];   /* only indices [0] and [1] are read here */
extern "C" dbgVAR_float dbg_crosshairDepth;
extern "C" dbgVAR_bool  dbg_hcex_off_hud;

extern int           hcex_cur_render_mode;
extern unsigned char hcex_render_ready;
extern "C" int           hcex_coop_local_player_index;
extern "C" int           hcex_off_cine_screen_effect;

/* --- ws-engine methods (free-function form; several are virtual dispatch) --- */
extern void osLOCK__Lock(struct osLOCK *lock, const char *file, int line);
extern void osLOCK_Unlock(struct osLOCK *lock, const char *file, int line);

extern void vidDRIVER_INTERFACE__EnsureVertShader(vidDRIVER_INTERFACE *self, void *shader);           /* virtual, vtbl+0x50 */
extern void vidDRIVER_INTERFACE__EnsurePixShader(vidDRIVER_INTERFACE *self, void *shader, int flag);  /* virtual, vtbl+0x54 */
extern void vidDRIVER_INTERFACE__EnsureShaderDecl(vidDRIVER_INTERFACE *self, void *decl,
        int stream_count, int force_rebuild); /* virtual, vtbl+0x5C — both flags are 0 at this call site */
extern void vidDRIVER_INTERFACE__ClearSource(vidDRIVER_INTERFACE *self);                              /* virtual, vtbl+0xD0 */

/* vidDRIVER::SetViewport (virtual, vtbl+0x98). DEVIATION: the decompiler named the 3rd/4th params
 * "width"/"height", but disassembly of the three call sites (including one with a nonzero y0) shows
 * they are the opposite corner (x1, y1), not a width/height pair — e.g. the split-screen bottom-half
 * call passes (x0=0, y0=sy/2, x1=sx, y1=sy), which only makes sense as absolute corners. */
extern void vidDRIVER__SetViewport(vidDRIVER *self, int x0, int y0, int x1, int y1, float minZ, float maxZ);

extern void d3dDRIVER_SetStartRenderingState(d3dDRIVER *self);

extern "C" int game_is_active(void);
extern "C" int16_t local_player_count(void);
extern void hcex_update_render_targets(int splitScreenIdx);
extern "C" void hcex_prepare_halo_render(int cam_idx);
extern "C" void hcex_update_screen_flash(int cam_idx);
extern "C" void interface_draw_screen(void);
extern "C" int  hcex_is_cine_mode(void);
extern "C" void rasterizer_screen_flash(void);
extern "C" void cinematic_render(void);
extern "C" void hud_render_timer(void);
extern "C" void terminal_draw(void);
extern "C" void raserizer_release_fx_textures(void);   /* HRESULT return, discarded by the caller */
