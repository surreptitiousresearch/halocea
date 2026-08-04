#ifndef _ID3DXEFFECTVTBL_H_
#define _ID3DXEFFECTVTBL_H_

/* Direct3DX effect object method table (COM vtable). DB-verified layout:
   types_members(ID3DXEffectVtbl), 79 entries x 4 bytes (0x13C total). Order is
   binary-significant: calls dispatch through effect->lpVtbl->Method by offset.
   All parameters beyond the primitive types are pointers to opaque D3DX/D3D
   boundary objects, forward-declared below. */

#include "d3d_boundary.h"        /* HRESULT, D3DDevice, D3DPixelShader, D3DVertexShader */
#include "D3DXVECTOR4.h"         /* D3DXVECTOR4 */
#include "ID3DXEffectPool.h"     /* ID3DXEffectPool */
#include "ID3DXEffect.h"         /* ID3DXEffect (self type) */

/* Opaque boundary types used only as pointers in the method table. */
typedef struct _GUID                    _GUID;
typedef struct _D3DXEFFECT_DESC         _D3DXEFFECT_DESC;
typedef struct _D3DXPARAMETER_DESC      _D3DXPARAMETER_DESC;
typedef struct _D3DXTECHNIQUE_DESC      _D3DXTECHNIQUE_DESC;
typedef struct _D3DXPASS_DESC           _D3DXPASS_DESC;
typedef struct _D3DXFUNCTION_DESC       _D3DXFUNCTION_DESC;
typedef struct _D3DMATRIX               _D3DMATRIX;
typedef struct D3DBaseTexture           D3DBaseTexture;
typedef struct ID3DXEffectStateManager  ID3DXEffectStateManager;

typedef struct ID3DXEffectVtbl
{
    HRESULT (__fastcall *QueryInterface)(ID3DXEffect *, const _GUID *const, void **); /* 0x00 */
    unsigned int (__fastcall *AddRef)(ID3DXEffect *); /* 0x04 */
    unsigned int (__fastcall *Release)(ID3DXEffect *); /* 0x08 */
    HRESULT (__fastcall *GetDesc)(ID3DXEffect *, _D3DXEFFECT_DESC *); /* 0x0C */
    HRESULT (__fastcall *GetParameterDesc)(ID3DXEffect *, unsigned int, _D3DXPARAMETER_DESC *); /* 0x10 */
    HRESULT (__fastcall *GetTechniqueDesc)(ID3DXEffect *, unsigned int, _D3DXTECHNIQUE_DESC *); /* 0x14 */
    HRESULT (__fastcall *GetPassDesc)(ID3DXEffect *, unsigned int, _D3DXPASS_DESC *); /* 0x18 */
    HRESULT (__fastcall *GetFunctionDesc)(ID3DXEffect *, unsigned int, _D3DXFUNCTION_DESC *); /* 0x1C */
    unsigned int (__fastcall *GetParameter)(ID3DXEffect *, unsigned int, unsigned int); /* 0x20 */
    unsigned int (__fastcall *GetParameterByName)(ID3DXEffect *, unsigned int, const char *); /* 0x24 */
    unsigned int (__fastcall *GetParameterBySemantic)(ID3DXEffect *, unsigned int, const char *); /* 0x28 */
    unsigned int (__fastcall *GetParameterElement)(ID3DXEffect *, unsigned int, unsigned int); /* 0x2C */
    unsigned int (__fastcall *GetTechnique)(ID3DXEffect *, unsigned int); /* 0x30 */
    unsigned int (__fastcall *GetTechniqueByName)(ID3DXEffect *, const char *); /* 0x34 */
    unsigned int (__fastcall *GetPass)(ID3DXEffect *, unsigned int, unsigned int); /* 0x38 */
    unsigned int (__fastcall *GetPassByName)(ID3DXEffect *, unsigned int, const char *); /* 0x3C */
    unsigned int (__fastcall *GetFunction)(ID3DXEffect *, unsigned int); /* 0x40 */
    unsigned int (__fastcall *GetFunctionByName)(ID3DXEffect *, const char *); /* 0x44 */
    unsigned int (__fastcall *GetAnnotation)(ID3DXEffect *, unsigned int, unsigned int); /* 0x48 */
    unsigned int (__fastcall *GetAnnotationByName)(ID3DXEffect *, unsigned int, const char *); /* 0x4C */
    HRESULT (__fastcall *SetValue)(ID3DXEffect *, unsigned int, const void *, unsigned int); /* 0x50 */
    HRESULT (__fastcall *GetValue)(ID3DXEffect *, unsigned int, void *, unsigned int); /* 0x54 */
    HRESULT (__fastcall *SetBool)(ID3DXEffect *, unsigned int, int); /* 0x58 */
    HRESULT (__fastcall *GetBool)(ID3DXEffect *, unsigned int, int *); /* 0x5C */
    HRESULT (__fastcall *SetBoolArray)(ID3DXEffect *, unsigned int, const int *, unsigned int); /* 0x60 */
    HRESULT (__fastcall *GetBoolArray)(ID3DXEffect *, unsigned int, int *, unsigned int); /* 0x64 */
    HRESULT (__fastcall *SetInt)(ID3DXEffect *, unsigned int, int); /* 0x68 */
    HRESULT (__fastcall *GetInt)(ID3DXEffect *, unsigned int, int *); /* 0x6C */
    HRESULT (__fastcall *SetIntArray)(ID3DXEffect *, unsigned int, const int *, unsigned int); /* 0x70 */
    HRESULT (__fastcall *GetIntArray)(ID3DXEffect *, unsigned int, int *, unsigned int); /* 0x74 */
    HRESULT (__fastcall *SetFloat)(ID3DXEffect *, unsigned int, float); /* 0x78 */
    HRESULT (__fastcall *GetFloat)(ID3DXEffect *, unsigned int, float *); /* 0x7C */
    HRESULT (__fastcall *SetFloatArray)(ID3DXEffect *, unsigned int, const float *, unsigned int); /* 0x80 */
    HRESULT (__fastcall *GetFloatArray)(ID3DXEffect *, unsigned int, float *, unsigned int); /* 0x84 */
    HRESULT (__fastcall *SetVector)(ID3DXEffect *, unsigned int, const D3DXVECTOR4 *); /* 0x88 */
    HRESULT (__fastcall *GetVector)(ID3DXEffect *, unsigned int, D3DXVECTOR4 *); /* 0x8C */
    HRESULT (__fastcall *SetVectorArray)(ID3DXEffect *, unsigned int, const D3DXVECTOR4 *, unsigned int); /* 0x90 */
    HRESULT (__fastcall *GetVectorArray)(ID3DXEffect *, unsigned int, D3DXVECTOR4 *, unsigned int); /* 0x94 */
    HRESULT (__fastcall *SetMatrix)(ID3DXEffect *, unsigned int, const _D3DMATRIX *); /* 0x98 */
    HRESULT (__fastcall *GetMatrix)(ID3DXEffect *, unsigned int, _D3DMATRIX *); /* 0x9C */
    HRESULT (__fastcall *SetMatrixArray)(ID3DXEffect *, unsigned int, const _D3DMATRIX *, unsigned int); /* 0xA0 */
    HRESULT (__fastcall *GetMatrixArray)(ID3DXEffect *, unsigned int, _D3DMATRIX *, unsigned int); /* 0xA4 */
    HRESULT (__fastcall *SetMatrixPointerArray)(ID3DXEffect *, unsigned int, const _D3DMATRIX **, unsigned int); /* 0xA8 */
    HRESULT (__fastcall *GetMatrixPointerArray)(ID3DXEffect *, unsigned int, _D3DMATRIX **, unsigned int); /* 0xAC */
    HRESULT (__fastcall *SetMatrixTranspose)(ID3DXEffect *, unsigned int, const _D3DMATRIX *); /* 0xB0 */
    HRESULT (__fastcall *GetMatrixTranspose)(ID3DXEffect *, unsigned int, _D3DMATRIX *); /* 0xB4 */
    HRESULT (__fastcall *SetMatrixTransposeArray)(ID3DXEffect *, unsigned int, const _D3DMATRIX *, unsigned int); /* 0xB8 */
    HRESULT (__fastcall *GetMatrixTransposeArray)(ID3DXEffect *, unsigned int, _D3DMATRIX *, unsigned int); /* 0xBC */
    HRESULT (__fastcall *SetMatrixTransposePointerArray)(ID3DXEffect *, unsigned int, const _D3DMATRIX **, unsigned int); /* 0xC0 */
    HRESULT (__fastcall *GetMatrixTransposePointerArray)(ID3DXEffect *, unsigned int, _D3DMATRIX **, unsigned int); /* 0xC4 */
    HRESULT (__fastcall *SetString)(ID3DXEffect *, unsigned int, const char *); /* 0xC8 */
    HRESULT (__fastcall *GetString)(ID3DXEffect *, unsigned int, const char **); /* 0xCC */
    HRESULT (__fastcall *SetTexture)(ID3DXEffect *, unsigned int, D3DBaseTexture *); /* 0xD0 */
    HRESULT (__fastcall *GetTexture)(ID3DXEffect *, unsigned int, D3DBaseTexture **); /* 0xD4 */
    HRESULT (__fastcall *SetPixelShader)(ID3DXEffect *, unsigned int, D3DPixelShader *); /* 0xD8 */
    HRESULT (__fastcall *GetPixelShader)(ID3DXEffect *, unsigned int, D3DPixelShader **); /* 0xDC */
    HRESULT (__fastcall *SetVertexShader)(ID3DXEffect *, unsigned int, D3DVertexShader *); /* 0xE0 */
    HRESULT (__fastcall *GetVertexShader)(ID3DXEffect *, unsigned int, D3DVertexShader **); /* 0xE4 */
    HRESULT (__fastcall *SetArrayRange)(ID3DXEffect *, unsigned int, unsigned int, unsigned int); /* 0xE8 */
    HRESULT (__fastcall *GetPool)(ID3DXEffect *, ID3DXEffectPool **); /* 0xEC */
    HRESULT (__fastcall *SetTechnique)(ID3DXEffect *, unsigned int); /* 0xF0 */
    unsigned int (__fastcall *GetCurrentTechnique)(ID3DXEffect *); /* 0xF4 */
    HRESULT (__fastcall *ValidateTechnique)(ID3DXEffect *, unsigned int); /* 0xF8 */
    HRESULT (__fastcall *FindNextValidTechnique)(ID3DXEffect *, unsigned int, unsigned int *); /* 0xFC */
    int (__fastcall *IsParameterUsed)(ID3DXEffect *, unsigned int, unsigned int); /* 0x100 */
    HRESULT (__fastcall *Begin)(ID3DXEffect *, unsigned int *, unsigned int); /* 0x104 */
    HRESULT (__fastcall *BeginPass)(ID3DXEffect *, unsigned int); /* 0x108 */
    HRESULT (__fastcall *CommitChanges)(ID3DXEffect *); /* 0x10C */
    HRESULT (__fastcall *EndPass)(ID3DXEffect *); /* 0x110 */
    HRESULT (__fastcall *End)(ID3DXEffect *); /* 0x114 */
    HRESULT (__fastcall *GetDevice)(ID3DXEffect *, D3DDevice **); /* 0x118 */
    HRESULT (__fastcall *OnLostDevice)(ID3DXEffect *); /* 0x11C */
    HRESULT (__fastcall *OnResetDevice)(ID3DXEffect *); /* 0x120 */
    HRESULT (__fastcall *SetStateManager)(ID3DXEffect *, ID3DXEffectStateManager *); /* 0x124 */
    HRESULT (__fastcall *GetStateManager)(ID3DXEffect *, ID3DXEffectStateManager **); /* 0x128 */
    HRESULT (__fastcall *BeginParameterBlock)(ID3DXEffect *); /* 0x12C */
    unsigned int (__fastcall *EndParameterBlock)(ID3DXEffect *); /* 0x130 */
    HRESULT (__fastcall *ApplyParameterBlock)(ID3DXEffect *, unsigned int); /* 0x134 */
    HRESULT (__fastcall *CloneEffect)(ID3DXEffect *, D3DDevice *, ID3DXEffect **); /* 0x138 */
} ID3DXEffectVtbl; /* 0x13C bytes */

#endif /* _ID3DXEFFECTVTBL_H_ */
