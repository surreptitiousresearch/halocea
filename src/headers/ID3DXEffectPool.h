#pragma once
#include "d3d_boundary.h" /* HRESULT */

/* CALLING CONVENTION — decided 2026-08-06, do not re-propose. These vtable slots are plain,
 * with no convention token. The SDK spells COM methods STDMETHODCALLTYPE, which is __stdcall
 * on x86 and nothing on Xbox 360 PowerPC — and PowerPC has exactly one calling convention, so
 * on the actual target there is nothing to annotate. The slots are only ever called through
 * the pointer and never linked by name, so no symbol depends on it either. The `__fastcall`
 * these carried until the callconv drain was IDA's label for the PPC register ABI, not source.
 * Adding __stdcall back would be a fresh x86 claim on a PPC reconstruction. */

/* Direct3DX effect-parameter pool — COM boundary interface.
   Opaque COM object: a single vtable pointer at offset 0. */
struct _GUID;
typedef struct ID3DXEffectPool ID3DXEffectPool;

/* DB-verified layout (types_members ID3DXEffectPoolVtbl): IUnknown triple, 12 bytes. */
typedef struct ID3DXEffectPoolVtbl
{
    HRESULT      (*QueryInterface)(ID3DXEffectPool *self, const struct _GUID *riid, void **out); /* 0x00 */
    unsigned int (*AddRef)(ID3DXEffectPool *self);                                                          /* 0x04 */
    unsigned int (*Release)(ID3DXEffectPool *self);                                                         /* 0x08 */
} ID3DXEffectPoolVtbl;

struct ID3DXEffectPool
{
    struct ID3DXEffectPoolVtbl *lpVtbl; /* 0x00 */
}; /* 4 bytes */
