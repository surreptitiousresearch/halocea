#pragma once
#include "d3d_boundary.h" /* HRESULT */

/* Direct3DX effect-parameter pool — COM boundary interface.
   Opaque COM object: a single vtable pointer at offset 0. */
struct _GUID;
typedef struct ID3DXEffectPool ID3DXEffectPool;

/* DB-verified layout (types_members ID3DXEffectPoolVtbl): IUnknown triple, 12 bytes. */
typedef struct ID3DXEffectPoolVtbl
{
    HRESULT      (__fastcall *QueryInterface)(ID3DXEffectPool *self, const struct _GUID *riid, void **out); /* 0x00 */
    unsigned int (__fastcall *AddRef)(ID3DXEffectPool *self);                                               /* 0x04 */
    unsigned int (__fastcall *Release)(ID3DXEffectPool *self);                                              /* 0x08 */
} ID3DXEffectPoolVtbl;

struct ID3DXEffectPool
{
    struct ID3DXEffectPoolVtbl *lpVtbl; /* 0x00 */
}; /* 4 bytes */
