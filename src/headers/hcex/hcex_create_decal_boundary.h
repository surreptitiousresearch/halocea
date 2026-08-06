#pragma once
/* Boundary declarations for hcex_create_decal.c.
 *
 * hcex_create_decal bridges a Blam decal-spawn request into the ws-engine: it converts the decal tag name,
 * rescales/reorients the Blam world position into ws space, and records an HCEX_DECAL in the hcexCreateDecals
 * vector (optionally logging the name mapping in hcexUsedDecals). The ws template containers (dsTSTRING_flat /
 * dsVECTOR) and the debug-variable system are extern boundaries per the project's re-source conventions;
 * only the fields the bridge touches are modeled, at their database offsets. The container "methods" are
 * declared as free functions whose first parameter is the object (mirroring the compiler-generated thiscall). */

#include "hcex_float3.h"   /* hcex_float3 (bridged Blam world position) */
#include "HCEX_DECAL.h"
#include "hcex_dbgvar_boundary.h"   /* dbgVAR_bool (value @ 0x0C), and dsTSTRING_flat/dlFree/UnsafeInit via hcex_ds_boundary.h */

typedef int dsCMP;

/* InsertSorted duplicate policy */
#define INS_DUP_IGNORE 0

/* opaque dsVECTOR<T,8> handles */
typedef struct dsVECTOR_DECAL dsVECTOR_DECAL;  /* hcexCreateDecals: dsVECTOR<HCEX_DECAL,8>   */
typedef struct dsVECTOR_TSTR  dsVECTOR_TSTR;   /* hcexUsedDecals:   dsVECTOR<dsTSTRING_flat,8>    */

/* --- debug-console toggles (dbgVAR_SIMPLE<bool,1>; the load site names ".value") --- */
extern "C" dbgVAR_bool dbg_hcex_create_decals;
extern "C" dbgVAR_bool dbg_hcexDumpUsedDecalsOn;

/* --- globals --- */
extern dsVECTOR_DECAL hcexCreateDecals;
extern dsVECTOR_TSTR  hcexUsedDecals;

/* --- ws container / string methods (dlFree, dsTSTRING_UnsafeInit already declared in hcex_ds_boundary.h) --- */
extern void        HCEX_DECAL_ctor(HCEX_DECAL *self);                                   /* HCEX_DECAL::HCEX_DECAL */
extern void        dsTSTRING_assign(dsTSTRING_flat *dst, const dsTSTRING_flat *src);              /* operator= */
extern void        dsTSTRING_Insert(dsTSTRING_flat *s, int at, const char *src, int len);
extern void        dsVECTOR_TSTR_InsertSorted(dsVECTOR_TSTR *v, const dsTSTRING_flat *s, dsCMP *cmp, int dup);
extern HCEX_DECAL *dsVECTOR_DECAL_PushBack(dsVECTOR_DECAL *v, const HCEX_DECAL *src);

/* --- CRT / hcex helpers --- */
extern "C" char *strncpy(char *dst, const char *src, unsigned int n);
extern "C" char *strrchr(const char *s, int c);
extern "C" void  hcex_conv_name(char *name, int max_len);
