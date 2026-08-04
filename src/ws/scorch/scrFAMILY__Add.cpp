// scrFAMILY::Add @0x82C95CA8 — admit one scorch/decal request into the scorch manager.
//
// LABEL DISCREPANCY: the ledger labels this symbol "scrFAMILY_BASE_Add", but the resolved function
// (mangled ?Add@scrFAMILY@@UAAH ABUscrSCORCH_INI@@ @Z) is scrFAMILY::Add — a virtual returning int
// and taking `const scrSCORCH_INI&`. Filed as scrFAMILY__Add.cpp accordingly.
//
// Flow: bail if new decals are disallowed or the surface is invalid; resolve the named brand out of
// the global scorch family; roll a randomized radius (from either the weighted cd buckets or the
// brand's min/max), recover the face normal if degenerate, cull against nearby existing marks and
// per-camera distance, roll texture index and rotation, then hand the finished ini to
// scrSCORCH_MANAGER::Add. See scrFAMILY__Add_boundary.h for the type layouts.
// DEVIATION 2026-08-04: the boundary header's earlier decompiler-inferred scrSCORCH_DESC /
// scrFAMILY layouts were wrong (cd/textures/rotAngleValues offsets, isAllowNewDecals at 0 instead
// of 0x8C); replaced with the DB types_members-verified canonical layouts. brandTable[7].pointee
// corresponds to dscBRAND::spDesc @ 0x1C (7 * sizeof(dsSMART_PTR)=4).
#include "../../headers/ws/scorch/scrFAMILY__Add_boundary.h"

int scrFAMILY::Add(const scrSCORCH_INI &iniRef) // DB mangling ABU = const reference
{
    const scrSCORCH_INI *ini = &iniRef; // body was written in pointer form
    if (!this->isAllowNewDecals)
        return 0;

    cdtINFO *cdtInfo = const_cast<cdtINFO *>(&ini->cdtInfo);
    if (!cdtINFO_IsValid(cdtInfo))
        return 0;
    cdtINFO_Checker(cdtInfo);
    if (((cdtInfo->state.state >> 8) & 1) == 0)   // require the "surface resolved" state bit
        return 0;

    // Resolve the brand smart-ptr; FindBrand returns the dsSMART_PTR table, slot 7 is the desc.
    dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > *brandTable =
        (dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > *)
            dscDESC_FAMILY_FindBrand(scrFamily, &ini->nameClass);
    if (!brandTable)
        return 0;

    scrSCORCH_DESC *desc = brandTable[7].pointee;

    // Hold an owning ref to the desc for the duration (compiler-generated refcount juggling).
    dsSMART_PTR<scrSCORCH_DESC, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > descRef(brandTable[7]);

    if (!desc->textures.nElem)                     // no textures -> nothing to place
        return 0;

    scrSCORCH_INI work(*ini);                       // working copy that gets randomized
    m3dRND_GEN rng;
    rng.seed = m3dRndGen.RndIntMax(32766);

    if (desc->cd.nElem <= 0)
    {
        work.radius = ini->radScale * rng.RndFloatRange(desc->radiusMin, desc->radiusMax);
    }
    else
    {
        // Weighted pick over the cd buckets: accumulate weights until we pass a uniform threshold.
        float threshold = rng.RndFloat();
        int i = 0;
        float accum = 0.0f;
        scrSCORCH_DESC::SCORCH_INI *bucket = nullptr;
        bool picked = false;
        for (i = 0; i < desc->cd.nElem; ++i)
        {
            bucket = &desc->cd[i];
            accum += bucket->weight;
            if (threshold < accum) { picked = true; break; }
        }
        if (picked)
            work.radius = rng.RndFloatRange(bucket->radMin, bucket->radMax);
        if (i == desc->cd.nElem)                    // fell through -> use last bucket
        {
            scrSCORCH_DESC::SCORCH_INI *last = &desc->cd.Back();
            work.radius = rng.RndFloatRange(last->radMin, last->radMax);
        }
    }

    work.radScale = 1.0f;
    if (__fabs(m3dLengthVector_2(&work.normal)) < M3D_EPSILON_2_309)
        cdtINFO_GetFaceNormal(&work.cdtInfo, &work.normal);

    dsSTRID *id = &desc->classID;
    float radNoAround = desc->radNoAround;
    float findRadius = desc->visibleAreaPart * work.radius;
    const m3dV *vClos = cdtINFO_GetVClos(cdtInfo);

    bool cull = false;
    // Cull if an existing mark is already too near, else run the per-camera distance test.
    if (scrSCORCH_MANAGER_FindAround(scrManager, id, vClos, findRadius, radNoAround, false))
    {
        cull = true;
    }
    else if (gsCameraCount() > 0)
    {
        for (int c = 0; ; ++c)
        {
            camCAMERA *cam = gsCameraGet(c);
            m3dV camPos;
            camPos.x = cam->matrC2W.elements[12];
            camPos.y = cam->matrC2W.elements[13];
            camPos.z = cam->matrC2W.elements[14];
            _m3dCheckValid(&camPos);
            float dist2 = m3dDist_2(cdtINFO_GetVClos(cdtInfo), &camPos);
            float degRadEnd = desc->degRadEnd;
            if (degRadEnd <= 0.0f) break;           // no distance gate -> accept
            _m3dCheckValid(&desc->degRadEnd);
            if (dist2 < degRadEnd * degRadEnd) break; // within gate -> accept
            if (c + 1 >= gsCameraCount()) { cull = true; break; } // beyond every camera -> cull
        }
    }
    else
    {
        cull = true;                                 // no cameras -> nothing sees it
    }

    if (cull)
        return 0;                                     // (work.nameClass buffer released on the way out)

    // Roll a texture index.
    int nTex = desc->textures.nElem;
    work.texIdx = nTex ? rng.RndIntMax(nTex - 1) : -1;

    // Roll rotation: explicit ini angle wins; else a discrete value, else jitter around brand angle.
    if (__fabs(ini->rotAngle) < 0.000001f)
    {
        if (desc->rotAngleValues.nElem > 0)
        {
            work.rotAngle = desc->rotAngleValues[rng.RndIntMax(desc->rotAngleValues.nElem - 1)];
        }
        else if (__fabs(desc->rotAngleDisp) < 0.000001f)
        {
            work.rotAngle = desc->rotAngle;
        }
        else
        {
            work.rotAngle = rng.RndFloatRange(desc->rotAngle - desc->rotAngleDisp,
                                              desc->rotAngle + desc->rotAngleDisp);
        }
    }

    scrSCORCH_MANAGER_Add(scrManager, &work, &descRef);
    return 1;
}
