#pragma once

#include <stdint.h>
#include "d3d_boundary.h"
#include "_D3DFORMAT.h"
#include "GPUTEXTURE_FETCH_CONSTANT.h"
#include "D3DBaseTexture.h"
#include "D3DTexture.h"
#include "_D3DQUERYTYPE.h"
#include "_D3DSURFACE_DESC.h"
#include "_D3DVIEWPORT9.h"
#include "_D3DPRIMITIVETYPE.h"
#include "_D3DTEXTURESTAGESTATETYPE.h"
#include "_D3DRESOURCETYPE.h"
#include "_D3DCUBEMAP_FACES.h"
#include "_D3DLOCKED_RECT.h"
#include "_D3DLOCKED_BOX.h"

/* Direct3D render-resource boundary types (Xbox 360 D3D SDK — not decompiled).
 *
 * Split pass: the individual D3D SDK types now live one-per-file (see #includes above); this
 * header aggregates them plus the extern SDK wrapper declarations. The opaque handle types
 * (D3DDevice/D3DSurface/D3DQuery/D3DVolumeTexture/D3DCubeTexture/D3DVertexBuffer/D3DIndexBuffer)
 * come transitively from d3d_boundary.h. The DB-verified _D3DFORMAT enum (real Xbox 360 tokens)
 * comes from _D3DFORMAT.h; the former inline placeholder enum was removed. */

#ifdef __cplusplus
extern "C" {
#endif
extern D3DSurface *D3DDevice_GetRenderTarget(D3DDevice *device, unsigned int RenderTargetIndex);
extern void D3DSurface_GetDesc(D3DSurface *surface, _D3DSURFACE_DESC *desc);
extern D3DQuery *D3DDevice_CreateQueryTiled(D3DDevice *device, _D3DQUERYTYPE type, unsigned int tiled);

extern void D3DDevice_SetViewport(D3DDevice *device, _D3DVIEWPORT9 *viewport);
extern void D3DDevice_Clear(D3DDevice *device, unsigned int count, const void *rects,
                            unsigned int flags, unsigned int color, float z,
                            unsigned int stencil, int a8);

/* Additional formats / ops for render-target setup. D3DFMT_A4R4G4B4 / D3DFMT_X8R8G8B8 / D3DFMT_A8R8G8B8
 * are real enumerators from the DB-verified _D3DFORMAT.h (no placeholder macros — a placeholder would
 * shadow the real enum value and break value-based switches such as hcex_conv_tex_fmt).
 * D3DFMT_UNKNOWN / D3DFMT_INDEX16 keep their legacy placeholder-macro values below: rasterizer_bitmap_new.c
 * relies on the historical UNKNOWN==0 sentinel, so these are intentionally NOT the _D3DFORMAT.h tokens. */
#define D3DFMT_UNKNOWN  ((_D3DFORMAT)0)
#define D3DFMT_INDEX16  ((_D3DFORMAT)101) /* placeholder value — PC D3D9 token; used only as the
                                              XGSetIndexBufferHeader Format arg (vidDEFRAG_POOL_HCEX_X360::CreateBuf) */
extern D3DSurface *D3DTexture_GetSurfaceLevel(D3DTexture *texture, unsigned int level);
extern unsigned int D3DResource_AddRef(struct D3DSurface *surface);

/* XDK resource-header helpers (boundary). Xbox 360 hardware buffer headers are laid out in place
 * over a caller-owned memory range rather than allocated by D3D; XGSet*BufferHeader fills the 0x20
 * byte header and XGOffsetResourceAddress points it at the physical base. Used by
 * vidDEFRAG_POOL_HCEX_X360::CreateBuf to alias one physical range as both IB and VB. */
extern void XGSetIndexBufferHeader(unsigned int Length, unsigned int Usage, _D3DFORMAT Format,
                                   unsigned int Pool, int Set, D3DIndexBuffer *pIndexBuffer);
extern void XGSetVertexBufferHeader(unsigned int Length, unsigned int Usage, unsigned int Pool,
                                    int Set, D3DVertexBuffer *pVertexBuffer);
extern void XGOffsetResourceAddress(D3DResource *pResource, void *pBaseAddress);

/* Sampler / stream state ops. The trailing PendingMask3 is the Xbox 360 GPU
 * pending-register bitmask for the slot being written (boundary types). */
extern void D3DDevice_SetTexture(D3DDevice *pDevice, unsigned int Sampler,
                                 D3DBaseTexture *pTexture, uint64_t PendingMask3);
extern void D3DDevice_SetStreamSource(D3DDevice *pDevice, unsigned int StreamNumber,
                                      D3DVertexBuffer *pVertexBuffer, unsigned int OffsetInBytes,
                                      unsigned int StrideInBytes, uint64_t PendingMask3);
extern void D3DDevice_SetIndices(D3DDevice *pDevice, D3DIndexBuffer *pIndexData);

extern void D3DDevice_DrawIndexedVertices(D3DDevice *pDevice, _D3DPRIMITIVETYPE PrimitiveType,
                                          unsigned int BaseVertexIndex, unsigned int StartIndex,
                                          unsigned int PrimitiveCount);

/* triangle_buffer.type (0=index-triangle-list, 1=index-triangle-strip) -> real D3D primitive type. Row
 * [0]=vertices-per-primitive multiplier, [1]=constant added, giving IndexCount = row[0]*count + row[1]. */
extern const unsigned int d3d_primitive_type_table[2];
extern const unsigned int D3DPRIMITIVEVERTEXCOUNT[14][2];

extern void SetTextureStageStateSmart(unsigned int stage, _D3DTEXTURESTAGESTATETYPE State, unsigned int Value);

/* Texture creation / lock boundary (Xbox 360 D3D SDK). D3DFMT_A4R4G4B4 comes from _D3DFORMAT.h. */
extern D3DBaseTexture *D3DDevice_CreateTexture(unsigned int Width, unsigned int Height, unsigned int Depth,
                                               unsigned int Levels, unsigned int Usage, _D3DFORMAT Format,
                                               unsigned int Unknown, _D3DRESOURCETYPE Type);
extern void D3DTexture_LockRect(D3DTexture *texture, unsigned int Level, _D3DLOCKED_RECT *pLockedRect,
                                const void *pRect, unsigned int Flags);
extern void D3DTexture_UnlockRect(D3DTexture *texture, unsigned int Level);
extern void D3DVolumeTexture_LockBox(D3DVolumeTexture *texture, unsigned int Level, _D3DLOCKED_BOX *pLockedBox,
                                     const void *pBox, unsigned int Flags);
extern void D3DVolumeTexture_UnlockBox(D3DVolumeTexture *texture, unsigned int Level);
extern void D3DCubeTexture_LockRect(D3DCubeTexture *texture, _D3DCUBEMAP_FACES FaceType, unsigned int Level,
                                    _D3DLOCKED_RECT *pLockedRect, const void *pRect, unsigned int Flags);
extern void D3DCubeTexture_UnlockRect(D3DCubeTexture *texture, _D3DCUBEMAP_FACES FaceType, unsigned int Level);
#ifdef __cplusplus
}
#endif
