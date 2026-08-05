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

/* cdtREFINE — DB-verified full layout (types_members cdtREFINE) — size 32. */
struct cdtREFINE_vtbl;  // boundary
struct animINST;        // ../animINST.h — pointer only
typedef struct cdtREFINE
{
    cdtREFINE_vtbl              *__vftable;       /* 0x00 */
    ds::WEAK_PTR_BASE<struct cdtREFINE> base;     /* 0x04 (anonymous base slot) */
    apSTATE_T<int64_t>          stateObjUsrIncl;  /* 0x08 */
    animINST                   *pInstSelf;        /* 0x10 */
    apSTATE_T<unsigned long>    state;            /* 0x14 */
    unsigned int                layerMask;        /* 0x18 */
    enum SELDOM { CENTROIDS_ONLY = 0, CENTERS_ONLY = 1, CENTROIDS_AND_CENTERS = 2, UNDEF = 3 } specMode; /* 0x1C */
} cdtREFINE;                 /* 32 bytes */

#define HCEX_DECAL_REFINE_STATE_OBJ_USR_INCL 0x000000C004000FBFULL /* raw ctor arg */

/* scrSCORCH_INI — DB-verified full layout (types_members scrSCORCH_INI) — size 84. */
struct atkDISPATCHER;   // atk subsystem — attack dispatcher (pointer only)
typedef struct scrSCORCH_INI
{
    msgDATA       base;         /* 0x00 (anonymous msgDATA base) */
    dsTSTRING<char>     nameClass;    /* 0x04 dsTSTRING<char> */
    cdtINFO       cdtInfo;      /* 0x08 (36B) */
    float         alphaScale;   /* 0x2C */
    float         radScale;     /* 0x30 */
    float         timeAdded;    /* 0x34 */
    float         radius;       /* 0x38 */
    float         rotAngle;     /* 0x3C */
    int           texIdx;       /* 0x40 */
    m3dV          normal;       /* 0x44 */
    const atkDISPATCHER *pAtkDisp; /* 0x50 */
} scrSCORCH_INI;                /* 84 bytes */

typedef struct scnSCENE scnSCENE;
extern scnSCENE *gsScenePtr;

typedef struct scrFAMILY_BASE scrFAMILY_BASE;
extern scrFAMILY_BASE *scrFamily;   /* the active scorch/decal family manager */

/* hcexCreateDecals: dsVECTOR<HCEX_DECAL,8> (20B) — spelled as the template-id (local flat
 * template) so the layout verifier's type match sees the DB member type. */
template<class T, int N> struct dsVECTOR { T *pData; int nElem; int allocated; unsigned char __cl[8]; };
extern dsVECTOR<HCEX_DECAL, 8> hcexCreateDecals;
extern void dsVECTOR_HCEX_DECAL_8__Clear(dsVECTOR<HCEX_DECAL, 8> *self);

/* --- ctor/dtor/methods (compiler-generated-thiscall free-function form) --- */
extern void scrSCORCH_INI_ctor(scrSCORCH_INI *self);          /* scrSCORCH_INI::scrSCORCH_INI(void) */
extern void dsTSTRING_assign(dsTSTRING<char> *dst, const dsTSTRING<char> *src); /* dsTSTRING<char>::operator= (shares buffer) */
extern void dsTSTRING_dtor(dsTSTRING<char> *s);                      /* dsTSTRING<char>::~dsTSTRING<char> (release buffer) */
extern void cdtREFINE_ctor(cdtREFINE *self, int flags, unsigned long layerMask,
        unsigned long long stateObjUsrIncl, void *pInstSelf); /* cdtREFINE::cdtREFINE(int,ulong,apSTATE_T<__int64>,animINST*) */
extern void cdtREFINE_dtor(cdtREFINE *self);                   /* cdtREFINE::~cdtREFINE(void) */
extern int  cdtINFO_GetFaceNormal(const cdtINFO *self, m3dV *out); /* cdtINFO::GetFaceNormal(m3dV*) const */
extern int  scnSCENE_GetClosestPoint_EXT(scnSCENE *scene, const m3dV *point, float radius,
        cdtREFINE *refine, cdtINFO *info);                     /* scnSCENE::GetClosestPoint_EXT */
extern int  scrFAMILY_BASE_Add(scrFAMILY_BASE *self, const scrSCORCH_INI *ini); /* scrFAMILY::Add (virtual) */

extern void m3dNormalize(m3dV *v);

extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent(void);
