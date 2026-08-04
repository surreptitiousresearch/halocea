#pragma once
/* Boundary types for hcex_resize_render_targets. The vid* driver and txm* texture manager are ws-engine
 * subsystems treated as extern boundaries; only the fields/methods this resize helper touches are modeled,
 * at DB offsets (vidDRIVER.pMode@0x10C, vidMODE.sx@0x10 / sy@0x14). txmD3D_TEX::Resize is the second vtable
 * slot (disasm: lwz r10,0(this); lwz r9,4(r10); bctrl) — modeled as a free function like GetD3DTex. */

#include "../txm_d3d_tex.h"   /* txmD3D_TEX, txmD3D_TEX__GetD3DTex, D3DTexture / D3DSurface */

/* vidMODE / vidDRIVER — canonical full DB-verified layouts (sx@0x10 / sy@0x14; pMode@0x10C);
 * the former per-consumer partial slices were consolidated. */
#include "../vidMODE.h"
#include "../vidDRIVER.h"

extern vidDRIVER  *vidDriver;
extern txmD3D_TEX *hcxe_global_render_targets_tex[];
extern void        txmD3D_TEX__Resize(txmD3D_TEX *self, int width, int height);
