#pragma once

#include <stdint.h>

/* Direct3D shader-resource boundary types (Xbox 360 D3D SDK — not decompiled). */
struct D3DDevice; /* fwd decl: without it, 'struct D3DDevice *' in the prototype below
                   * gets prototype-only tag scope (C11 6.2.1p4) and conflicts with
                   * d3d_boundary.h's decl in TUs that include both. */
typedef struct D3DResource D3DResource;
typedef struct D3DVertexShader D3DVertexShader;
typedef struct D3DVertexDeclaration D3DVertexDeclaration;

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned int D3DResource_Release(D3DResource *resource);

extern D3DVertexShader *D3DDevice_CreateVertexShader(const unsigned int *function);

extern void D3DDevice_SetVertexShaderConstantFN(struct D3DDevice *device, unsigned int reg,
                                                const float *data, unsigned int vector4f_count,
                                                uint64_t pending_mask0); /* DB: Vector4fCount + unsigned __int64 PendingMask0 (disasm-verified @0x8369D15C/8369D1B8) */
#ifdef __cplusplus
}
#endif
