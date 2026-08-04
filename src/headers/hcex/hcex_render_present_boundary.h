#pragma once
/* Boundary declarations for hcex_render_present.c.
 *
 * hcex_render_present is the HCEX bridge's end-of-frame present hook: it flushes the new UI proxy through the
 * ws render path, and — when running in Halo mode with stereo-3D active — resolves the split-screen render
 * target and re-composites it as two vertical half-screen quads before swapping buffers. Every subsystem it
 * touches (ui_new, d3d/vid drivers, txm manager, rend driver, gs render system) is a large self-contained
 * ws-engine class treated as an extern boundary per the project's re-source conventions; only the fields the
 * bridge reads directly are modeled, at their database offsets. Virtual-dispatch calls are declared as free
 * functions taking the object as the first parameter (the corpus convention). */

/* --- opaque ws-engine driver / manager / system classes --- */
typedef struct ui_new_UI_PROXY   ui_new_UI_PROXY;
typedef struct d3dDRIVER         d3dDRIVER;
typedef struct rendDRIVER        rendDRIVER;
typedef struct txmTEXTURE        txmTEXTURE;

/* --- ws-engine driver / manager / render-target classes: canonical full DB-verified layouts
 * (the former per-consumer partial slices were consolidated) --- */
#include "../vidMODE.h"
#include "../vidDRIVER.h"
#include "../ws/txm/txmMANAGER.h"
#include "../ws/gs/gsRENDER_SYSTEM.h"
#include "../ws/txm/txmD3D_TEX.h"

/* RenderQuad blend mode (PASS_DESC_BASE::BLENDMODE); only the default (0) is used here. */
typedef int BLENDMODE;
#define BLENDMODE_DEFAULT 0

/* --- globals --- */
extern ui_new_UI_PROXY *ui_new_uiProxy;                 /* ui_new::uiProxy */
extern d3dDRIVER       *d3dDriver;
extern vidDRIVER       *vidDriver;
extern txmMANAGER      *txmManager;
extern rendDRIVER      *rendDrv;
extern gsRENDER_SYSTEM *gsSysRender;
extern txmD3D_TEX     **hcxe_global_render_targets_tex;

/* --- ws-engine methods (free-function form; several are virtual dispatch) --- */
extern void d3dDRIVER_SetStartRenderingState(d3dDRIVER *self);
extern void ui_new_UI_PROXY_RenderPush(ui_new_UI_PROXY *self);
extern void ui_new_UI_PROXY_ProcessRender(ui_new_UI_PROXY *self);
extern void vidDRIVER_BeginScene(vidDRIVER *self);                                    /* virtual */
extern void vidDRIVER_EndScene(vidDRIVER *self);                                      /* virtual */
extern void vidDRIVER_CopyBackBufferToTexture(vidDRIVER *self, txmTEXTURE *dst,       /* virtual (+0xA0) */
                                              int x, int y, int w, int h);
extern int  vidDRIVER_SetRenderTarget(vidDRIVER *self, txmTEXTURE *pTexRend,
                                      txmTEXTURE *pTexDepth, unsigned int stateDefUse); /* apSTATE_T<ulong> passed by value */
extern void rendDRIVER_RenderQuad(rendDRIVER *self,
                                  float x1, float y1, float s1, float t1,
                                  float x2, float y2, float s2, float t2,
                                  float unused9, BLENDMODE blend, unsigned int mask,
                                  txmTEXTURE *texture);
extern void gsRENDER_SYSTEM_SwapBuffers(gsRENDER_SYSTEM *self, int a, int b);

/* --- OS / bridge helpers --- */
extern void osPIXBeginEvent(const char *label);
extern void osPIXEndEvent(void);
extern int  hcex_is_halo_mode(void);
