#pragma once
/* Boundary declarations for hcex_create_decals_delayed.c.
 *
 * hcex_create_decals_delayed drains the hcexCreateDecals queue built up over the frame: for each queued
 * decal it re-probes the world surface under the decal's position (scnSCENE::GetClosestPoint_EXT) to get
 * a fresh face normal, builds a scrSCORCH_INI scorch/decal descriptor from it, and hands that to the
 * active scrFAMILY_BASE (the ws-engine scorch-decal manager) via Add(). The cdt (collision-detection)
 * query types, the scr scorch-family manager, and the m3d math library are ws-engine boundaries. */

#include <stdint.h>
#include "HCEX_DECAL.h"
#include "hcex_ds_boundary.h"          /* dsTSTRING<char> (flat), dsTSTRING_assign-style helpers */
#include "../ws/m3d/m3dV.h"
#include "../ws/cdt/cdtINFO.h"         /* cdtINFO — canonical full DB-verified layout */
#include "../ws/ap/apSTATE_T.h"
#include "../ws/ds/WEAK_PTR.h"
#include "../ws/ds/WEAK_PTR_BASE.h"   /* ds::WEAK_PTR_BASE<cdtREFINE> base slot */
#include "../ws/msg/msgDATA.h"

/* cdtREFINE — DB-verified full layout (types_members cdtREFINE) — size 32. The canonical ws
 * definition is used directly; its `weakSelf` is the member this file used to spell `base`
 * (types_members marks that ds::WEAK_PTR_BASE<cdtREFINE> slot is_baseclass=1, unnamed). */
#include "../ws/cdt/cdtREFINE.h"

/* Raw 64-bit apSTATE_T<__int64> ctor argument (the value `ld r6, 0(r24)` loads at 0x823DDF40).
 * Spelled LL rather than ULL so it needs no conversion at the apSTATE_T<int64_t> call below;
 * the bit pattern is unchanged (it is positive in 64 bits). */
#define HCEX_DECAL_REFINE_STATE_OBJ_USR_INCL 0x000000C004000FBFLL

/* scrSCORCH_INI — DB-verified full layout (types_members scrSCORCH_INI) — size 84; canonical ws
 * definition used directly (identical member set/offsets, plus its ctor declarations). */
#include "../ws/scorch/scrSCORCH_INI.h"

typedef struct scnSCENE scnSCENE;
extern scnSCENE *gsScenePtr;

typedef struct scrFAMILY_BASE scrFAMILY_BASE;
extern scrFAMILY_BASE *scrFamily;   /* the active scorch/decal family manager */

/* hcexCreateDecals: dsVECTOR<HCEX_DECAL,8> (20B) — spelled as the template-id so the layout
 * verifier's type match sees the DB member type; the template itself is the canonical ws one
 * (pData@0, nElem@4, allocated@8, __cl@12 (apCL, 8B) — the former local shim spelled that last
 * member `unsigned char __cl[8]`, same 20-byte layout). */
#include "../ws/ds/dsVECTOR.h"
extern dsVECTOR<HCEX_DECAL, 8> hcexCreateDecals;
extern void dsVECTOR_HCEX_DECAL_8__Clear(dsVECTOR<HCEX_DECAL, 8> *self);

/* --- ctor/dtor/methods (compiler-generated-thiscall free-function form) ---
 * cdtREFINE's ctor/dtor are NOT declared here any more: the canonical header carries the real
 * member declarations, so the consumer constructs/destroys the object directly. Its ctor is the
 * 4-argument (int flags, unsigned long layerMask, apSTATE_T<__int64>, animINST *) form — see the
 * DEVIATION note in ws/cdt/cdtREFINE.h; this file's free-function declaration was the only copy
 * of that arity in the tree. */
extern void dsTSTRING_assign(dsTSTRING<char> *dst, const dsTSTRING<char> *src); /* dsTSTRING<char>::operator= (shares buffer) */
extern void dsTSTRING_dtor(dsTSTRING<char> *s);                      /* dsTSTRING<char>::~dsTSTRING<char> (release buffer) */
extern int  cdtINFO_GetFaceNormal(const cdtINFO *self, m3dV *out); /* cdtINFO::GetFaceNormal(m3dV*) const */
extern int  scnSCENE_GetClosestPoint_EXT(scnSCENE *scene, const m3dV *point, float radius,
        cdtREFINE *refine, cdtINFO *info);                     /* scnSCENE::GetClosestPoint_EXT */
extern int  scrFAMILY_BASE_Add(scrFAMILY_BASE *self, const scrSCORCH_INI *ini); /* scrFAMILY::Add (virtual) */

extern void m3dNormalize(m3dV *v);

extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent(void);
