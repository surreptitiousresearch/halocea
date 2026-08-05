#pragma once
/* Boundary declarations for hcex_tex_get_hardware_format.c.
 *
 * The function forces a ws-engine texture resident, marks it used, and (when it still has no hardware
 * surface) seeds the streaming accumulator so the streamer prioritises it, optionally kicking a blocking
 * stream. txm* (texture manager), strm* (streaming resource) and the D3D base texture are ws-engine / D3D
 * boundaries: only the touched fields are modeled, at their DB-verified offsets (types_members).
 *
 * txmD3D_TEX embeds txmTEXTURE at offset 0, so tex->name (dsTSTRING_flat @0x04) and tex->strmResource
 * (strmRESOURCE_TEXTURE* @0x90 == 144) are txmTEXTURE members. strmRESOURCE_TEXTURE::accDataVector is at
 * +104, its lastData (ACCUM_DATA, 32 bytes) at +192 within that: mipCoef/coefDist/dist2 are m2dV{x,y},
 * splitCount @0x18. */

#include "hcex_dbgvar_boundary.h"   /* dbgVAR_bool, dsTSTRING_flat (flat), dsTSTRING_BUF_HEADER, dlFree */
#include "../apCL.h"                /* apCL */
/* strmRESOURCE_TEXTURE and txmD3D_TEX — canonical full DB-verified ws headers (the former
 * per-consumer partial slices were consolidated). */
#include "../ws/strm/strmRESOURCE_TEXTURE.h"
#include "../ws/txm/txmD3D_TEX.h"
#include "../ws/ds/dsTSTRING.h"     /* dsTSTRING<char> (txmTEXTURE::name / hcexTexList element) */
#include "../ws/ds/dsCMP.h"         /* canonical dsCMP (stateless) — avoids C2371 vs int typedef */
#include "../ws/ds/dsVECTOR.h"      /* canonical dsVECTOR — avoids C2953 vs local template */

/* InsertSorted duplicate policy */
#define INS_DUP_IGNORE 0

typedef struct D3DBaseTexture D3DBaseTexture;   /* D3D boundary, opaque */

/* --- ws texture manager (opaque) --- */
typedef struct txmMANAGER txmMANAGER;
extern txmMANAGER *txmManager;

extern D3DBaseTexture *txmD3D_TEX__GetD3DTex(txmD3D_TEX *self);               /* txmD3D_TEX::GetD3DTex */
extern int             txmMANAGER_Load(txmMANAGER *self, txmD3D_TEX *tex, int force_resident); /* txmMANAGER::Load */
extern void            txmTEXTURE__MarkAsUsedInRender(txmD3D_TEX *tex);       /* txmTEXTURE::MarkAsUsedInRender */

/* --- streaming texture manager (singleton) --- */
typedef struct strmRES_TEX_MANAGER strmRES_TEX_MANAGER;
extern strmRES_TEX_MANAGER strmRES_TEX_MANAGER_mng;                           /* strmRES_TEX_MANAGER::mng */
extern void strmRES_TEX_MANAGER_NecessaryStreamTexture(strmRES_TEX_MANAGER *mng, txmD3D_TEX *tex);

/* --- debug: dump-used-textures list --- */
extern dbgVAR_bool          dbg_hcexDumpUsedTexOn;
/* hcexTexList: dsVECTOR<dsTSTRING<char>,8> (20B) — canonical dsVECTOR (included above). */
extern dsVECTOR<dsTSTRING<char>, 8> hcexTexList;
extern void dsVECTOR_dsTSTRING_InsertSorted(dsVECTOR<dsTSTRING<char>, 8> *v,
        const dsTSTRING<char> *s, dsCMP *cmp, int dup_policy);

/* --- config toggle + strong-assert + PIX profiling boundaries --- */
extern int  hcex_tex_block;             /* int hcex_tex_block — force blocking stream */
extern int IGNORE_STRONG_ASSERT; /* .data @0x841DB148 - ?IGNORE_STRONG_ASSERT@@3HA (def: src/data/IGNORE_STRONG_ASSERT.cpp) */
extern "C" const char empty_string[]; /* .rdata @0x8200155A - the shared "" literal (def: src/data/empty_string.c) */
extern void STRONG_ASSERT_DUMMY_Crash(void *ctx, const char *expr, const char *file, int line, const char *info); /* STRONG_ASSERT_DUMMY::Crash */
extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent(void);
extern int  _snprintf_0(char *buf, unsigned int size, const char *fmt, ...);
