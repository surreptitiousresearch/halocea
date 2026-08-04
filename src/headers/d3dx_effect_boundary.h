#pragma once

/* Direct3DX effect-framework boundary — free-function declarations for the D3DX
   effect API. The boundary interface/data types this file used to bundle now
   live in one-type-per-file headers (SPLIT PASS); they are pulled in below and
   the free functions that reference them remain here. */
#include "d3dx_boundary.h"           /* ID3DXBuffer */
#include "D3DXVECTOR4.h"
#include "_D3DXMACRO.h"
#include "ID3DXEffectPool.h"
#include "ID3DXEffect.h"
#include "ID3DXEffectCompiler.h"

extern long D3DXCreateEffectPool(ID3DXEffectPool **pool);

struct D3DDevice;
extern long D3DXCreateEffectFromFileA(struct D3DDevice *device, const char *file,
                                      const _D3DXMACRO *defines, void *include,
                                      unsigned int flags, ID3DXEffectPool *pool,
                                      ID3DXEffect **effect, ID3DXBuffer **errors);

extern long D3DXCreateEffect(struct D3DDevice *device, const void *data, unsigned int size,
                             const _D3DXMACRO *defines, void *include, unsigned int flags,
                             ID3DXEffectPool *pool, ID3DXEffect **effect, ID3DXBuffer **errors);

extern long D3DXCreateEffectCompilerFromFileA(const char *file, const _D3DXMACRO *defines,
                                              void *include, unsigned int flags,
                                              ID3DXEffectCompiler **compiler, ID3DXBuffer **errors);

/* --- ID3DXEffect / ID3DXEffectCompiler C boundary wrappers ---------------
   The COM objects are opaque ({ lpVtbl }) so their methods cannot be reached
   member-style in C. These free functions mirror the D3DX9 method table; each
   takes the object as its first argument, so a call site rewrites 1:1 from
   obj->Method(obj, args) to ID3DXEffect_Method(obj, args). Parameter-handle
   values are the 32-bit D3DXHANDLE the DB models as unsigned int (matches
   rasterizer_dx9_shader.constants[]/texture[]). */
#ifndef _ID3DXEFFECT_C_WRAPPERS_
#define _ID3DXEFFECT_C_WRAPPERS_
extern unsigned int ID3DXEffect_GetTechniqueByName(ID3DXEffect *effect, const char *name);
extern long ID3DXEffect_ValidateTechnique(ID3DXEffect *effect, unsigned int technique);
extern long ID3DXEffect_FindNextValidTechnique(ID3DXEffect *effect, unsigned int technique,
                                               unsigned int *next);
extern long ID3DXEffect_SetTechnique(ID3DXEffect *effect, unsigned int technique);
extern unsigned int ID3DXEffect_GetParameterByName(ID3DXEffect *effect, unsigned int parent,
                                                   const char *name);
extern long ID3DXEffect_SetVector(ID3DXEffect *effect, unsigned int parameter,
                                  const D3DXVECTOR4 *vector);
extern long ID3DXEffect_SetTexture(ID3DXEffect *effect, unsigned int parameter, void *texture);
extern long ID3DXEffect_Begin(ID3DXEffect *effect, unsigned int *passes, unsigned int flags);
extern long ID3DXEffect_BeginPass(ID3DXEffect *effect, unsigned int pass);
extern long ID3DXEffect_EndPass(ID3DXEffect *effect);
extern long ID3DXEffect_End(ID3DXEffect *effect);
extern unsigned long ID3DXEffect_Release(ID3DXEffect *effect);

extern long ID3DXEffectCompiler_CompileEffect(ID3DXEffectCompiler *compiler, unsigned int flags,
                                              ID3DXBuffer **effect, ID3DXBuffer **errors); /* fix: CompileEffect yields the bytecode as an ID3DXBuffer, not ID3DXEffect */
extern unsigned long ID3DXEffectCompiler_Release(ID3DXEffectCompiler *compiler);
#endif /* _ID3DXEFFECT_C_WRAPPERS_ */
