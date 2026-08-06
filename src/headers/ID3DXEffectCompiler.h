#pragma once
#include "d3d_boundary.h" /* HRESULT, D3D handle types */

/* CALLING CONVENTION — decided 2026-08-06, do not re-propose. These vtable slots are plain,
 * with no convention token. The SDK spells COM methods STDMETHODCALLTYPE, which is __stdcall
 * on x86 and nothing on Xbox 360 PowerPC — and PowerPC has exactly one calling convention, so
 * on the actual target there is nothing to annotate. The slots are only ever called through
 * the pointer and never linked by name, so no symbol depends on it either. The `__fastcall`
 * these carried until the callconv drain was IDA's label for the PPC register ABI, not source.
 * Adding __stdcall back would be a fresh x86 claim on a PPC reconstruction. */

/* Direct3DX effect compiler — COM boundary interface (dev/tool path).
   DB-verified layout (types_members ID3DXEffectCompilerVtbl): 63 slots,
   252 bytes. Parameter/desc types below are D3DX boundary types. */

struct _GUID;
struct _D3DXEFFECT_DESC;
struct _D3DXPARAMETER_DESC;
struct _D3DXTECHNIQUE_DESC;
struct _D3DXPASS_DESC;
struct _D3DXFUNCTION_DESC;
struct D3DXVECTOR4;
struct _D3DMATRIX;
struct D3DBaseTexture;
struct D3DPixelShader;
struct D3DVertexShader;
struct ID3DXBuffer;
struct ID3DXConstantTable;
typedef struct ID3DXEffectCompiler ID3DXEffectCompiler;

typedef struct ID3DXEffectCompilerVtbl
{
    /* IUnknown */
    HRESULT      (*QueryInterface)(ID3DXEffectCompiler *self, const struct _GUID *riid, void **out); /* 0x00 */
    unsigned int (*AddRef)(ID3DXEffectCompiler *self);                                                          /* 0x04 */
    unsigned int (*Release)(ID3DXEffectCompiler *self);                                                         /* 0x08 */
    /* ID3DXBaseEffect descriptors */
    HRESULT (*GetDesc)(ID3DXEffectCompiler *self, struct _D3DXEFFECT_DESC *desc);                                       /* 0x0C */
    HRESULT (*GetParameterDesc)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DXPARAMETER_DESC *desc); /* 0x10 */
    HRESULT (*GetTechniqueDesc)(ID3DXEffectCompiler *self, unsigned int hTech, struct _D3DXTECHNIQUE_DESC *desc);             /* 0x14 */
    HRESULT (*GetPassDesc)(ID3DXEffectCompiler *self, unsigned int hPass, struct _D3DXPASS_DESC *desc);                       /* 0x18 */
    HRESULT (*GetFunctionDesc)(ID3DXEffectCompiler *self, unsigned int hFunc, struct _D3DXFUNCTION_DESC *desc);               /* 0x1C */
    /* handle lookup */
    unsigned int (*GetParameter)(ID3DXEffectCompiler *self, unsigned int hParent, unsigned int index);                   /* 0x20 */
    unsigned int (*GetParameterByName)(ID3DXEffectCompiler *self, unsigned int hParent, const char *name);               /* 0x24 */
    unsigned int (*GetParameterBySemantic)(ID3DXEffectCompiler *self, unsigned int hParent, const char *sem); /* 0x28 */
    unsigned int (*GetParameterElement)(ID3DXEffectCompiler *self, unsigned int hParent, unsigned int index); /* 0x2C */
    unsigned int (*GetTechnique)(ID3DXEffectCompiler *self, unsigned int index);                                         /* 0x30 */
    unsigned int (*GetTechniqueByName)(ID3DXEffectCompiler *self, const char *name);                                     /* 0x34 */
    unsigned int (*GetPass)(ID3DXEffectCompiler *self, unsigned int hTech, unsigned int index);                          /* 0x38 */
    unsigned int (*GetPassByName)(ID3DXEffectCompiler *self, unsigned int hTech, const char *name);                      /* 0x3C */
    unsigned int (*GetFunction)(ID3DXEffectCompiler *self, unsigned int index);                                          /* 0x40 */
    unsigned int (*GetFunctionByName)(ID3DXEffectCompiler *self, const char *name);                                      /* 0x44 */
    unsigned int (*GetAnnotation)(ID3DXEffectCompiler *self, unsigned int hObj, unsigned int index);                     /* 0x48 */
    unsigned int (*GetAnnotationByName)(ID3DXEffectCompiler *self, unsigned int hObj, const char *name);                 /* 0x4C */
    /* value setters/getters */
    HRESULT (*SetValue)(ID3DXEffectCompiler *self, unsigned int hParam, const void *data, unsigned int bytes); /* 0x50 */
    HRESULT (*GetValue)(ID3DXEffectCompiler *self, unsigned int hParam, void *data, unsigned int bytes);                  /* 0x54 */
    HRESULT (*SetBool)(ID3DXEffectCompiler *self, unsigned int hParam, int b);                                            /* 0x58 */
    HRESULT (*GetBool)(ID3DXEffectCompiler *self, unsigned int hParam, int *b);                                           /* 0x5C */
    HRESULT (*SetBoolArray)(ID3DXEffectCompiler *self, unsigned int hParam, const int *b, unsigned int n);                /* 0x60 */
    HRESULT (*GetBoolArray)(ID3DXEffectCompiler *self, unsigned int hParam, int *b, unsigned int n);                      /* 0x64 */
    HRESULT (*SetInt)(ID3DXEffectCompiler *self, unsigned int hParam, int v);                                             /* 0x68 */
    HRESULT (*GetInt)(ID3DXEffectCompiler *self, unsigned int hParam, int *v);                                            /* 0x6C */
    HRESULT (*SetIntArray)(ID3DXEffectCompiler *self, unsigned int hParam, const int *v, unsigned int n);                 /* 0x70 */
    HRESULT (*GetIntArray)(ID3DXEffectCompiler *self, unsigned int hParam, int *v, unsigned int n);                       /* 0x74 */
    HRESULT (*SetFloat)(ID3DXEffectCompiler *self, unsigned int hParam, float v);                                         /* 0x78 */
    HRESULT (*GetFloat)(ID3DXEffectCompiler *self, unsigned int hParam, float *v);                                        /* 0x7C */
    HRESULT (*SetFloatArray)(ID3DXEffectCompiler *self, unsigned int hParam, const float *v, unsigned int n);             /* 0x80 */
    HRESULT (*GetFloatArray)(ID3DXEffectCompiler *self, unsigned int hParam, float *v, unsigned int n);                   /* 0x84 */
    HRESULT (*SetVector)(ID3DXEffectCompiler *self, unsigned int hParam, const struct D3DXVECTOR4 *v);                    /* 0x88 */
    HRESULT (*GetVector)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DXVECTOR4 *v);                          /* 0x8C */
    HRESULT (*SetVectorArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct D3DXVECTOR4 *v, unsigned int n); /* 0x90 */
    HRESULT (*GetVectorArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DXVECTOR4 *v, unsigned int n);                  /* 0x94 */
    HRESULT (*SetMatrix)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX *m);                     /* 0x98 */
    HRESULT (*GetMatrix)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX *m);                           /* 0x9C */
    HRESULT (*SetMatrixArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX *m, unsigned int n); /* 0xA0 */
    HRESULT (*GetMatrixArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX *m, unsigned int n);                  /* 0xA4 */
    HRESULT (*SetMatrixPointerArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX **m, unsigned int n); /* 0xA8 */
    HRESULT (*GetMatrixPointerArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX **m, unsigned int n);                  /* 0xAC */
    HRESULT (*SetMatrixTranspose)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX *m); /* 0xB0 */
    HRESULT (*GetMatrixTranspose)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX *m);                  /* 0xB4 */
    HRESULT (*SetMatrixTransposeArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX *m, unsigned int n); /* 0xB8 */
    HRESULT (*GetMatrixTransposeArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX *m, unsigned int n);                  /* 0xBC */
    HRESULT (*SetMatrixTransposePointerArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX **m, unsigned int n); /* 0xC0 */
    HRESULT (*GetMatrixTransposePointerArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX **m, unsigned int n);                  /* 0xC4 */
    HRESULT (*SetString)(ID3DXEffectCompiler *self, unsigned int hParam, const char *s);                                  /* 0xC8 */
    HRESULT (*GetString)(ID3DXEffectCompiler *self, unsigned int hParam, const char **s);                                 /* 0xCC */
    HRESULT (*SetTexture)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DBaseTexture *t);                      /* 0xD0 */
    HRESULT (*GetTexture)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DBaseTexture **t);                     /* 0xD4 */
    HRESULT (*SetPixelShader)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DPixelShader *ps);                 /* 0xD8 */
    HRESULT (*GetPixelShader)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DPixelShader **ps);                /* 0xDC */
    HRESULT (*SetVertexShader)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DVertexShader *vs);               /* 0xE0 */
    HRESULT (*GetVertexShader)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DVertexShader **vs);              /* 0xE4 */
    HRESULT (*SetArrayRange)(ID3DXEffectCompiler *self, unsigned int hParam, unsigned int start, unsigned int end); /* 0xE8 */
    /* ID3DXEffectCompiler */
    HRESULT (*SetLiteral)(ID3DXEffectCompiler *self, unsigned int hParam, int literal);                                   /* 0xEC */
    HRESULT (*GetLiteral)(ID3DXEffectCompiler *self, unsigned int hParam, int *literal);                                  /* 0xF0 */
    HRESULT (*CompileEffect)(ID3DXEffectCompiler *self, unsigned int flags, struct ID3DXBuffer **effect, struct ID3DXBuffer **errors); /* 0xF4 */
    HRESULT (*CompileShader)(ID3DXEffectCompiler *self, unsigned int hFunc, const char *target, unsigned int flags, struct ID3DXBuffer **shader, struct ID3DXBuffer **errors, struct ID3DXConstantTable **ctab); /* 0xF8 */
} ID3DXEffectCompilerVtbl; /* 252 bytes, 63 slots */

struct ID3DXEffectCompiler
{
    struct ID3DXEffectCompilerVtbl *lpVtbl; /* 0x0 */
}; /* 4 bytes */
