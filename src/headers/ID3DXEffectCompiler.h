#pragma once
#include "d3d_boundary.h" /* HRESULT, D3D handle types */

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
    HRESULT      (__fastcall *QueryInterface)(ID3DXEffectCompiler *self, const struct _GUID *riid, void **out); /* 0x00 */
    unsigned int (__fastcall *AddRef)(ID3DXEffectCompiler *self);                                               /* 0x04 */
    unsigned int (__fastcall *Release)(ID3DXEffectCompiler *self);                                              /* 0x08 */
    /* ID3DXBaseEffect descriptors */
    HRESULT (__fastcall *GetDesc)(ID3DXEffectCompiler *self, struct _D3DXEFFECT_DESC *desc);                            /* 0x0C */
    HRESULT (__fastcall *GetParameterDesc)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DXPARAMETER_DESC *desc); /* 0x10 */
    HRESULT (__fastcall *GetTechniqueDesc)(ID3DXEffectCompiler *self, unsigned int hTech, struct _D3DXTECHNIQUE_DESC *desc);  /* 0x14 */
    HRESULT (__fastcall *GetPassDesc)(ID3DXEffectCompiler *self, unsigned int hPass, struct _D3DXPASS_DESC *desc);            /* 0x18 */
    HRESULT (__fastcall *GetFunctionDesc)(ID3DXEffectCompiler *self, unsigned int hFunc, struct _D3DXFUNCTION_DESC *desc);    /* 0x1C */
    /* handle lookup */
    unsigned int (__fastcall *GetParameter)(ID3DXEffectCompiler *self, unsigned int hParent, unsigned int index);        /* 0x20 */
    unsigned int (__fastcall *GetParameterByName)(ID3DXEffectCompiler *self, unsigned int hParent, const char *name);    /* 0x24 */
    unsigned int (__fastcall *GetParameterBySemantic)(ID3DXEffectCompiler *self, unsigned int hParent, const char *sem); /* 0x28 */
    unsigned int (__fastcall *GetParameterElement)(ID3DXEffectCompiler *self, unsigned int hParent, unsigned int index); /* 0x2C */
    unsigned int (__fastcall *GetTechnique)(ID3DXEffectCompiler *self, unsigned int index);                              /* 0x30 */
    unsigned int (__fastcall *GetTechniqueByName)(ID3DXEffectCompiler *self, const char *name);                          /* 0x34 */
    unsigned int (__fastcall *GetPass)(ID3DXEffectCompiler *self, unsigned int hTech, unsigned int index);               /* 0x38 */
    unsigned int (__fastcall *GetPassByName)(ID3DXEffectCompiler *self, unsigned int hTech, const char *name);           /* 0x3C */
    unsigned int (__fastcall *GetFunction)(ID3DXEffectCompiler *self, unsigned int index);                               /* 0x40 */
    unsigned int (__fastcall *GetFunctionByName)(ID3DXEffectCompiler *self, const char *name);                           /* 0x44 */
    unsigned int (__fastcall *GetAnnotation)(ID3DXEffectCompiler *self, unsigned int hObj, unsigned int index);          /* 0x48 */
    unsigned int (__fastcall *GetAnnotationByName)(ID3DXEffectCompiler *self, unsigned int hObj, const char *name);      /* 0x4C */
    /* value setters/getters */
    HRESULT (__fastcall *SetValue)(ID3DXEffectCompiler *self, unsigned int hParam, const void *data, unsigned int bytes); /* 0x50 */
    HRESULT (__fastcall *GetValue)(ID3DXEffectCompiler *self, unsigned int hParam, void *data, unsigned int bytes);       /* 0x54 */
    HRESULT (__fastcall *SetBool)(ID3DXEffectCompiler *self, unsigned int hParam, int b);                                 /* 0x58 */
    HRESULT (__fastcall *GetBool)(ID3DXEffectCompiler *self, unsigned int hParam, int *b);                                /* 0x5C */
    HRESULT (__fastcall *SetBoolArray)(ID3DXEffectCompiler *self, unsigned int hParam, const int *b, unsigned int n);     /* 0x60 */
    HRESULT (__fastcall *GetBoolArray)(ID3DXEffectCompiler *self, unsigned int hParam, int *b, unsigned int n);           /* 0x64 */
    HRESULT (__fastcall *SetInt)(ID3DXEffectCompiler *self, unsigned int hParam, int v);                                  /* 0x68 */
    HRESULT (__fastcall *GetInt)(ID3DXEffectCompiler *self, unsigned int hParam, int *v);                                 /* 0x6C */
    HRESULT (__fastcall *SetIntArray)(ID3DXEffectCompiler *self, unsigned int hParam, const int *v, unsigned int n);      /* 0x70 */
    HRESULT (__fastcall *GetIntArray)(ID3DXEffectCompiler *self, unsigned int hParam, int *v, unsigned int n);            /* 0x74 */
    HRESULT (__fastcall *SetFloat)(ID3DXEffectCompiler *self, unsigned int hParam, float v);                              /* 0x78 */
    HRESULT (__fastcall *GetFloat)(ID3DXEffectCompiler *self, unsigned int hParam, float *v);                             /* 0x7C */
    HRESULT (__fastcall *SetFloatArray)(ID3DXEffectCompiler *self, unsigned int hParam, const float *v, unsigned int n);  /* 0x80 */
    HRESULT (__fastcall *GetFloatArray)(ID3DXEffectCompiler *self, unsigned int hParam, float *v, unsigned int n);        /* 0x84 */
    HRESULT (__fastcall *SetVector)(ID3DXEffectCompiler *self, unsigned int hParam, const struct D3DXVECTOR4 *v);         /* 0x88 */
    HRESULT (__fastcall *GetVector)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DXVECTOR4 *v);               /* 0x8C */
    HRESULT (__fastcall *SetVectorArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct D3DXVECTOR4 *v, unsigned int n); /* 0x90 */
    HRESULT (__fastcall *GetVectorArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DXVECTOR4 *v, unsigned int n);       /* 0x94 */
    HRESULT (__fastcall *SetMatrix)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX *m);          /* 0x98 */
    HRESULT (__fastcall *GetMatrix)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX *m);                /* 0x9C */
    HRESULT (__fastcall *SetMatrixArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX *m, unsigned int n); /* 0xA0 */
    HRESULT (__fastcall *GetMatrixArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX *m, unsigned int n);       /* 0xA4 */
    HRESULT (__fastcall *SetMatrixPointerArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX **m, unsigned int n); /* 0xA8 */
    HRESULT (__fastcall *GetMatrixPointerArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX **m, unsigned int n);       /* 0xAC */
    HRESULT (__fastcall *SetMatrixTranspose)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX *m); /* 0xB0 */
    HRESULT (__fastcall *GetMatrixTranspose)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX *m);       /* 0xB4 */
    HRESULT (__fastcall *SetMatrixTransposeArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX *m, unsigned int n); /* 0xB8 */
    HRESULT (__fastcall *GetMatrixTransposeArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX *m, unsigned int n);       /* 0xBC */
    HRESULT (__fastcall *SetMatrixTransposePointerArray)(ID3DXEffectCompiler *self, unsigned int hParam, const struct _D3DMATRIX **m, unsigned int n); /* 0xC0 */
    HRESULT (__fastcall *GetMatrixTransposePointerArray)(ID3DXEffectCompiler *self, unsigned int hParam, struct _D3DMATRIX **m, unsigned int n);       /* 0xC4 */
    HRESULT (__fastcall *SetString)(ID3DXEffectCompiler *self, unsigned int hParam, const char *s);                       /* 0xC8 */
    HRESULT (__fastcall *GetString)(ID3DXEffectCompiler *self, unsigned int hParam, const char **s);                      /* 0xCC */
    HRESULT (__fastcall *SetTexture)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DBaseTexture *t);           /* 0xD0 */
    HRESULT (__fastcall *GetTexture)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DBaseTexture **t);          /* 0xD4 */
    HRESULT (__fastcall *SetPixelShader)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DPixelShader *ps);      /* 0xD8 */
    HRESULT (__fastcall *GetPixelShader)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DPixelShader **ps);     /* 0xDC */
    HRESULT (__fastcall *SetVertexShader)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DVertexShader *vs);    /* 0xE0 */
    HRESULT (__fastcall *GetVertexShader)(ID3DXEffectCompiler *self, unsigned int hParam, struct D3DVertexShader **vs);   /* 0xE4 */
    HRESULT (__fastcall *SetArrayRange)(ID3DXEffectCompiler *self, unsigned int hParam, unsigned int start, unsigned int end); /* 0xE8 */
    /* ID3DXEffectCompiler */
    HRESULT (__fastcall *SetLiteral)(ID3DXEffectCompiler *self, unsigned int hParam, int literal);                        /* 0xEC */
    HRESULT (__fastcall *GetLiteral)(ID3DXEffectCompiler *self, unsigned int hParam, int *literal);                       /* 0xF0 */
    HRESULT (__fastcall *CompileEffect)(ID3DXEffectCompiler *self, unsigned int flags, struct ID3DXBuffer **effect, struct ID3DXBuffer **errors); /* 0xF4 */
    HRESULT (__fastcall *CompileShader)(ID3DXEffectCompiler *self, unsigned int hFunc, const char *target, unsigned int flags, struct ID3DXBuffer **shader, struct ID3DXBuffer **errors, struct ID3DXConstantTable **ctab); /* 0xF8 */
} ID3DXEffectCompilerVtbl; /* 252 bytes, 63 slots */

struct ID3DXEffectCompiler
{
    struct ID3DXEffectCompilerVtbl *lpVtbl; /* 0x0 */
}; /* 4 bytes */
