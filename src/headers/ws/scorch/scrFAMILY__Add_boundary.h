#pragma once
// Boundary declarations for scrFAMILY::Add (src/ws/scorch/scrFAMILY__Add.cpp).
//
// scrFAMILY::Add is the ws-engine scorch/decal manager's per-decal admission path: given a
// scrSCORCH_INI request it validates the surface (cdtINFO), resolves the named scorch brand out of
// the global scorch family (dscDESC_FAMILY::FindBrand), rolls randomized radius/rotation/texture
// from the resolved scrSCORCH_DESC, culls against nearby existing marks and camera distance, and
// finally enqueues the finished ini into scrSCORCH_MANAGER.
//
// 2026-08-04: previously carried decompiler-inferred duplicate layouts for scrSCORCH_DESC /
// scrFAMILY (SABER2 escalation #2). The DB DOES have types_members for both; the duplicates are
// replaced by the canonical DB-verified headers, and scrFAMILY below matches types_members
// scrFAMILY (base scrFAMILY_BASE = dscDESC_FAMILY, 0x8C bytes; isAllowNewDecals int @ 0x8C).
#include "scrSCORCH_INI.h"
#include "scrSCORCH_DESC.h"
#include "../dsc/dscDESC_FAMILY.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dRND_GEN.h"
#include "../cam/camCAMERA.h"
#include "../cdt/cdtINFO.h"

struct dsREF_COUNT;

// DB-verified (types_members scrFAMILY): base scrFAMILY_BASE (= dscDESC_FAMILY, 0x8C incl. pad),
// isAllowNewDecals @ 0x8C. Add is virtual (vtbl slot 6, ?Add@scrFAMILY@@UAAHABUscrSCORCH_INI@@@Z).
typedef struct scrFAMILY : dscDESC_FAMILY
{
    // base subobject sizes to 0x8C (bool @ 0x88 + natural pad); no explicit pad needed
    int           isAllowNewDecals; // 0x8C

    int Add(const scrSCORCH_INI &ini);   // 0x82C95CA8
} scrFAMILY;

typedef struct scrSCORCH_MANAGER scrSCORCH_MANAGER; // opaque here (canonical: scrSCORCH_MANAGER.h)

// --- globals ---
extern dscDESC_FAMILY *scrFamily;       // the scorch brand family (FindBrand source)
extern scrSCORCH_MANAGER *scrManager;   // the active scorch-mark store
extern m3dRND_GEN m3dRndGen;            // shared RNG

// --- ws-engine free/boundary functions the body calls ---
extern void *dscDESC_FAMILY_FindBrand(dscDESC_FAMILY *family, const dsTSTRING<char> *name);
extern bool cdtINFO_IsValid(const cdtINFO *self);
extern void cdtINFO_Checker(cdtINFO *self);
extern void cdtINFO_GetFaceNormal(cdtINFO *self, m3dV *out);
extern const m3dV *cdtINFO_GetVClos(cdtINFO *self);
extern bool scrSCORCH_MANAGER_FindAround(scrSCORCH_MANAGER *mgr, dsSTRID *id, const m3dV *pos,
        float radius, float radNoAround, bool arg);
extern void scrSCORCH_MANAGER_Add(scrSCORCH_MANAGER *mgr, scrSCORCH_INI *ini, void *smartPtr);
extern int  gsCameraCount(void);
extern camCAMERA *gsCameraGet(int idx);
extern float m3dLengthVector_2(const m3dV *v);
extern float m3dDist_2(const m3dV *a, const m3dV *b);
extern void _m3dCheckValid(const void *v);
extern void dlFree(void *ptr);

#define M3D_EPSILON_2_309 2.30909e-4f
