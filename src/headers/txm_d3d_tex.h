#pragma once

#include "d3d_render_boundary.h"   /* D3DTexture, D3DSurface */
#include "ws/txm/txmD3D_TEX.h"     /* txmD3D_TEX -- canonical full DB-verified layout */

/* Plain-C API surface over the hardware render-target texture wrapper (txm subsystem).
   The former local partial slice of txmD3D_TEX was consolidated onto the canonical
   ws/txm/txmD3D_TEX.h. */

extern D3DTexture *txmD3D_TEX__GetD3DTex(txmD3D_TEX *self);

/* txmD3D_TEX::EnableGamma — vtable slot 3 (offset 0x0C), inherited from txmTEXTURE's virtual table
   (DB-verified via the txmD3D_TEX / txmTEXTURE vtbl member rows). Toggles hardware gamma-correct
   sampling on the render target's texture. */
extern void txmD3D_TEX__EnableGamma(txmD3D_TEX *self, int enable);
