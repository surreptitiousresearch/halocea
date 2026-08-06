#pragma once

/* Texture-manager subsystem (txm*) boundary surface for the plain-C rasterizer/hcex
   consumers. The former local partial slices (txmTEXTURE opaque view, mis-numbered ETADDR
   placeholder, txmWHITE helper) were consolidated onto the canonical DB-verified ws/txm
   headers: txmTEXTURE (full layout incl. refCount@0x50 and pSplitScreenTex), the real
   ENGINE_TEX_ADDRESS_MODE enum (ETADDR_WRAP=0, ETADDR_CLAMP=1, ... — DB-verified; the old
   placeholder wrongly numbered CLAMP=0), and txmTEXTURE_PTR (the DB type behind the old
   txmWHITE helper shape). */

#include "ws/txm/txmTEXTURE.h"      /* txmTEXTURE (full DB layout) */
#include "ws/txm/ENGINE_TEX_DESC.h" /* ENGINE_TEX_ADDRESS_MODE / ETADDR_* */
#include "ws/txm/txmTEXTURE_PTR.h"  /* txmTEXTURE_PTR (ptr@0) */

typedef ENGINE_TEX_ADDRESS_MODE ETADDR; /* legacy spelling used by the C call sites */

typedef struct txmMANAGER txmMANAGER;

extern txmTEXTURE *txmMANAGER__Add(txmMANAGER *self, const char *name, int flags,
                                   ETADDR addr_u, ETADDR addr_v, int a6);
extern void *txmMANAGER__Find(txmMANAGER *self, const char *name);

/* Globals */
extern txmMANAGER *txmManager;
extern "C" int hcex_off_tex_streaming;
// DB mangled `?osLockedIncrement@@YAHPAH@Z` → int(int*); was wrongly `long` (conflicted with the
// canonical decl under strict C++). Return width unchanged on this ABI; callers unaffected.
extern int osLockedIncrement(int *value);

/* whiteTex fallback is reached via txmManager->whiteTex.ptr; expose an accessor
   shape without committing to the full txmMANAGER layout. */
extern txmTEXTURE_PTR *txmMANAGER_white_tex(txmMANAGER *self);

/* txmTEXTURE::GetSplitScreenTexture (non-virtual, QBA) — returns the split-screen sub-texture at
   `idx` (0/1) from txmTEXTURE::pSplitScreenTex[2]; asserts 0<=idx<=1 unless IGNORE_STRONG_ASSERT.
   Callers reach it through a txmD3D_TEX* (upcast to txmTEXTURE*) since txmD3D_TEX derives from
   txmTEXTURE at offset 0. */
extern txmTEXTURE *txmTEXTURE__GetSplitScreenTexture(txmTEXTURE *self, unsigned int idx);
