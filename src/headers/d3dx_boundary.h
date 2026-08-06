#pragma once

/* Direct3DX boundary types (shader compiler) — not decompiled. COM interface per
   the DB: ID3DXBuffer is a single lpVtbl pointer to ID3DXBufferVtbl (C-style
   COM); methods take an explicit `this`. */
typedef struct ID3DXConstantTable ID3DXConstantTable;
typedef struct ID3DXBuffer ID3DXBuffer;
typedef struct _GUID _GUID;

typedef struct ID3DXBufferVtbl
{
    long (*QueryInterface)(ID3DXBuffer *self, const _GUID *const riid, void **ppv); /* 0x00 — HRESULT */
    unsigned int (*AddRef)(ID3DXBuffer *self);                                      /* 0x04 */
    unsigned int (*Release)(ID3DXBuffer *self);                                     /* 0x08 */
    void *(*GetBufferPointer)(ID3DXBuffer *self);                                   /* 0x0C */
    unsigned int (*GetBufferSize)(ID3DXBuffer *self);                               /* 0x10 */
} ID3DXBufferVtbl; /* 20 bytes */

struct ID3DXBuffer
{
    ID3DXBufferVtbl *lpVtbl; /* 0x00 */
};

/* Reached by both C and C++ TUs. The D3DX exports carry C linkage in the real SDK headers and the
   symbol is flat in this binary, so the C++ view must not mangle it. */
#ifdef __cplusplus
extern "C" {
#endif

extern long D3DXCompileShaderFromFileA(const char *file, const void *defines,
                                       void *include, const char *entry,
                                       const char *profile, unsigned int flags,
                                       ID3DXBuffer **shader, ID3DXBuffer **errors,
                                       ID3DXConstantTable **table);

#ifdef __cplusplus
}
#endif
