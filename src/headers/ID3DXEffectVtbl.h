#ifndef _ID3DXEFFECTVTBL_H_
#define _ID3DXEFFECTVTBL_H_

/* CALLING CONVENTION — decided 2026-08-06, do not re-propose. These vtable slots are plain,
 * with no convention token. The SDK spells COM methods STDMETHODCALLTYPE, which is __stdcall
 * on x86 and nothing on Xbox 360 PowerPC — and PowerPC has exactly one calling convention, so
 * on the actual target there is nothing to annotate. The slots are only ever called through
 * the pointer and never linked by name, so no symbol depends on it either. The `__fastcall`
 * these carried until the callconv drain was IDA's label for the PPC register ABI, not source.
 * Adding __stdcall back would be a fresh x86 claim on a PPC reconstruction. */

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
    HRESULT (*QueryInterface)(ID3DXEffect *, const _GUID *const, void **); /* 0x00 */
    unsigned int (*AddRef)(ID3DXEffect *); /* 0x04 */
    unsigned int (*Release)(ID3DXEffect *); /* 0x08 */
    HRESULT (*GetDesc)(ID3DXEffect *, _D3DXEFFECT_DESC *); /* 0x0C */
    HRESULT (*GetParameterDesc)(ID3DXEffect *, unsigned int, _D3DXPARAMETER_DESC *); /* 0x10 */
    HRESULT (*GetTechniqueDesc)(ID3DXEffect *, unsigned int, _D3DXTECHNIQUE_DESC *); /* 0x14 */
    HRESULT (*GetPassDesc)(ID3DXEffect *, unsigned int, _D3DXPASS_DESC *); /* 0x18 */
    HRESULT (*GetFunctionDesc)(ID3DXEffect *, unsigned int, _D3DXFUNCTION_DESC *); /* 0x1C */
    unsigned int (*GetParameter)(ID3DXEffect *, unsigned int, unsigned int); /* 0x20 */
    unsigned int (*GetParameterByName)(ID3DXEffect *, unsigned int, const char *); /* 0x24 */
    unsigned int (*GetParameterBySemantic)(ID3DXEffect *, unsigned int, const char *); /* 0x28 */
    unsigned int (*GetParameterElement)(ID3DXEffect *, unsigned int, unsigned int); /* 0x2C */
    unsigned int (*GetTechnique)(ID3DXEffect *, unsigned int); /* 0x30 */
    unsigned int (*GetTechniqueByName)(ID3DXEffect *, const char *); /* 0x34 */
    unsigned int (*GetPass)(ID3DXEffect *, unsigned int, unsigned int); /* 0x38 */
    unsigned int (*GetPassByName)(ID3DXEffect *, unsigned int, const char *); /* 0x3C */
    unsigned int (*GetFunction)(ID3DXEffect *, unsigned int); /* 0x40 */
    unsigned int (*GetFunctionByName)(ID3DXEffect *, const char *); /* 0x44 */
    unsigned int (*GetAnnotation)(ID3DXEffect *, unsigned int, unsigned int); /* 0x48 */
    unsigned int (*GetAnnotationByName)(ID3DXEffect *, unsigned int, const char *); /* 0x4C */
    HRESULT (*SetValue)(ID3DXEffect *, unsigned int, const void *, unsigned int); /* 0x50 */
    HRESULT (*GetValue)(ID3DXEffect *, unsigned int, void *, unsigned int); /* 0x54 */
    HRESULT (*SetBool)(ID3DXEffect *, unsigned int, int); /* 0x58 */
    HRESULT (*GetBool)(ID3DXEffect *, unsigned int, int *); /* 0x5C */
    HRESULT (*SetBoolArray)(ID3DXEffect *, unsigned int, const int *, unsigned int); /* 0x60 */
    HRESULT (*GetBoolArray)(ID3DXEffect *, unsigned int, int *, unsigned int); /* 0x64 */
    HRESULT (*SetInt)(ID3DXEffect *, unsigned int, int); /* 0x68 */
    HRESULT (*GetInt)(ID3DXEffect *, unsigned int, int *); /* 0x6C */
    HRESULT (*SetIntArray)(ID3DXEffect *, unsigned int, const int *, unsigned int); /* 0x70 */
    HRESULT (*GetIntArray)(ID3DXEffect *, unsigned int, int *, unsigned int); /* 0x74 */
    HRESULT (*SetFloat)(ID3DXEffect *, unsigned int, float); /* 0x78 */
    HRESULT (*GetFloat)(ID3DXEffect *, unsigned int, float *); /* 0x7C */
    HRESULT (*SetFloatArray)(ID3DXEffect *, unsigned int, const float *, unsigned int); /* 0x80 */
    HRESULT (*GetFloatArray)(ID3DXEffect *, unsigned int, float *, unsigned int); /* 0x84 */
    HRESULT (*SetVector)(ID3DXEffect *, unsigned int, const D3DXVECTOR4 *); /* 0x88 */
    HRESULT (*GetVector)(ID3DXEffect *, unsigned int, D3DXVECTOR4 *); /* 0x8C */
    HRESULT (*SetVectorArray)(ID3DXEffect *, unsigned int, const D3DXVECTOR4 *, unsigned int); /* 0x90 */
    HRESULT (*GetVectorArray)(ID3DXEffect *, unsigned int, D3DXVECTOR4 *, unsigned int); /* 0x94 */
    HRESULT (*SetMatrix)(ID3DXEffect *, unsigned int, const _D3DMATRIX *); /* 0x98 */
    HRESULT (*GetMatrix)(ID3DXEffect *, unsigned int, _D3DMATRIX *); /* 0x9C */
    HRESULT (*SetMatrixArray)(ID3DXEffect *, unsigned int, const _D3DMATRIX *, unsigned int); /* 0xA0 */
    HRESULT (*GetMatrixArray)(ID3DXEffect *, unsigned int, _D3DMATRIX *, unsigned int); /* 0xA4 */
    HRESULT (*SetMatrixPointerArray)(ID3DXEffect *, unsigned int, const _D3DMATRIX **, unsigned int); /* 0xA8 */
    HRESULT (*GetMatrixPointerArray)(ID3DXEffect *, unsigned int, _D3DMATRIX **, unsigned int); /* 0xAC */
    HRESULT (*SetMatrixTranspose)(ID3DXEffect *, unsigned int, const _D3DMATRIX *); /* 0xB0 */
    HRESULT (*GetMatrixTranspose)(ID3DXEffect *, unsigned int, _D3DMATRIX *); /* 0xB4 */
    HRESULT (*SetMatrixTransposeArray)(ID3DXEffect *, unsigned int, const _D3DMATRIX *, unsigned int); /* 0xB8 */
    HRESULT (*GetMatrixTransposeArray)(ID3DXEffect *, unsigned int, _D3DMATRIX *, unsigned int); /* 0xBC */
    HRESULT (*SetMatrixTransposePointerArray)(ID3DXEffect *, unsigned int, const _D3DMATRIX **, unsigned int); /* 0xC0 */
    HRESULT (*GetMatrixTransposePointerArray)(ID3DXEffect *, unsigned int, _D3DMATRIX **, unsigned int); /* 0xC4 */
    HRESULT (*SetString)(ID3DXEffect *, unsigned int, const char *); /* 0xC8 */
    HRESULT (*GetString)(ID3DXEffect *, unsigned int, const char **); /* 0xCC */
    HRESULT (*SetTexture)(ID3DXEffect *, unsigned int, D3DBaseTexture *); /* 0xD0 */
    HRESULT (*GetTexture)(ID3DXEffect *, unsigned int, D3DBaseTexture **); /* 0xD4 */
    HRESULT (*SetPixelShader)(ID3DXEffect *, unsigned int, D3DPixelShader *); /* 0xD8 */
    HRESULT (*GetPixelShader)(ID3DXEffect *, unsigned int, D3DPixelShader **); /* 0xDC */
    HRESULT (*SetVertexShader)(ID3DXEffect *, unsigned int, D3DVertexShader *); /* 0xE0 */
    HRESULT (*GetVertexShader)(ID3DXEffect *, unsigned int, D3DVertexShader **); /* 0xE4 */
    HRESULT (*SetArrayRange)(ID3DXEffect *, unsigned int, unsigned int, unsigned int); /* 0xE8 */
    HRESULT (*GetPool)(ID3DXEffect *, ID3DXEffectPool **); /* 0xEC */
    HRESULT (*SetTechnique)(ID3DXEffect *, unsigned int); /* 0xF0 */
    unsigned int (*GetCurrentTechnique)(ID3DXEffect *); /* 0xF4 */
    HRESULT (*ValidateTechnique)(ID3DXEffect *, unsigned int); /* 0xF8 */
    HRESULT (*FindNextValidTechnique)(ID3DXEffect *, unsigned int, unsigned int *); /* 0xFC */
    int (*IsParameterUsed)(ID3DXEffect *, unsigned int, unsigned int); /* 0x100 */
    HRESULT (*Begin)(ID3DXEffect *, unsigned int *, unsigned int); /* 0x104 */
    HRESULT (*BeginPass)(ID3DXEffect *, unsigned int); /* 0x108 */
    HRESULT (*CommitChanges)(ID3DXEffect *); /* 0x10C */
    HRESULT (*EndPass)(ID3DXEffect *); /* 0x110 */
    HRESULT (*End)(ID3DXEffect *); /* 0x114 */
    HRESULT (*GetDevice)(ID3DXEffect *, D3DDevice **); /* 0x118 */
    HRESULT (*OnLostDevice)(ID3DXEffect *); /* 0x11C */
    HRESULT (*OnResetDevice)(ID3DXEffect *); /* 0x120 */
    HRESULT (*SetStateManager)(ID3DXEffect *, ID3DXEffectStateManager *); /* 0x124 */
    HRESULT (*GetStateManager)(ID3DXEffect *, ID3DXEffectStateManager **); /* 0x128 */
    HRESULT (*BeginParameterBlock)(ID3DXEffect *); /* 0x12C */
    unsigned int (*EndParameterBlock)(ID3DXEffect *); /* 0x130 */
    HRESULT (*ApplyParameterBlock)(ID3DXEffect *, unsigned int); /* 0x134 */
    HRESULT (*CloneEffect)(ID3DXEffect *, D3DDevice *, ID3DXEffect **); /* 0x138 */
} ID3DXEffectVtbl; /* 0x13C bytes */

#endif /* _ID3DXEFFECTVTBL_H_ */
