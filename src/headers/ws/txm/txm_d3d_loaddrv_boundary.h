#pragma once

/* Boundary surface for txmD3D_TEX::LoadDrv (src/ws/txm/txmD3D_TEX__LoadDrv.cpp): the pct pixel
 * copy helper, the Xbox 360 D3D surface-desc getter, the video-driver lock, and the app log --
 * all external to this batch. The pct format/size helpers come from pct_boundary.h and the D3D
 * lock/unlock wrappers from d3d_render_boundary.h. */

#include "../../pct_boundary.h"        /* pctPICTURE / pctFORMAT / pctGetMemSize / pctGetFormat */
#include "../../d3d_render_boundary.h" /* D3D lock/unlock wrappers, _D3DSURFACE_DESC, D3DTexture */
#include "../vid/vidLOCK.h"            /* vidLOCK */

/* The global video-driver critical section guarding all lock/upload sequences. boundary. */
extern vidLOCK vidLock;

/* ?pctCopyBuf@@YAXPAUpctPICTURE@@HHPAUpctFORMAT@@HPAEH@Z -- copy/convert one (face, mip) level of
 * a picture into a locked driver surface: `pitch` is the destination row pitch, `dst` the locked
 * pixel pointer, `slicePitch` the destination slice pitch (0 for 2D/cube). boundary. */
extern void pctCopyBuf(pctPICTURE *pic, int face, int mip, pctFORMAT *fmt, int pitch,
                       unsigned char *dst, int slicePitch);

/* Xbox 360 D3D: fetch the surface descriptor for a texture mip level. boundary. */
extern void D3DTexture_GetLevelDesc(D3DTexture *texture, unsigned int Level, _D3DSURFACE_DESC *desc);

/* App log (variadic). boundary. */
void _apLog(const char *format, ...);
