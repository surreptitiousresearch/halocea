/* ?Init@animLOD@@QAAXXZ @0x82B0E760 */
#include "animLOD_LOD_INFO.h"
#include "animINST.h"
#include "animTPL.h"
#include "tplLOD_DEF.h"
#include "objOBJ_boundary.h"
#include "anim_boundary.h"
#include "../ds/dsVECTOR.h"
#include "../ps/psSECTION.h"
#include "../ps/psSECTION_INTERFACE.h"
#include "../ps/psARRAY.h"
#include "../m3d/m3d_boundary.h"
#include <math.h>

// CAVEAT: the DB flagged "local variable allocation has failed" for this function. The two spots
// that needed untangling:
//   1. The instance's ps-section handle (a phantom `animINST v39`) is really the by-value psSECTION
//      returned by animINST::GetPS, reinterpreted through psSECTION_INTERFACE (same 4-byte layout).
//   2. The distance loop packs an int->float conversion through a stack slot (PowerPC has no direct
//      GPR->FPR move): numerator = (float)(level+1), denominator = (float)(maxLod-1), and the pow()
//      call was surfaced as one packed `long double`; it is pow((level+1)/(maxLod-1), gamma).
// Also: the decompiler mistyped `pInst->ppObjList[objId]` (an objOBJ*) as a psSECTION_DATA*, so its
// `->pOwner` (offset 8) is objOBJ::pGeom and `->psParents.__cl.line` (offset 28) is
// objOBJ::stateProc; both are modeled through the real objOBJ fields here.

// ?Init@animLOD@@QAAXXZ  (0x82B0E760) — build the level-of-detail table for the owning instance:
// bucket each template LOD object into its level (and every higher level when it is the last LOD),
// then compute the per-level switch distances from the ps "LOD" section (explicit "dist" array, or
// a maxDist/gamma power curve), finishing with the transition zone and the initial LOD selection.
void animLOD::Init()
{
    animTPL *pTpl = this->pInst->pTpl;
    dsVECTOR<tplLOD_DEF, 8> *pLodDef = &pTpl->lodDef;

    // Pass 1: highest LOD level referenced, plus one -> number of LOD buckets.
    int lodCount = 0;
    for (int i = 0; i < pTpl->lodDef.nElem; ++i)
    {
        const tplLOD_DEF *def = &(*pLodDef)[i];
        if (lodCount < def->index + 1)
            lodCount = def->index + 1;
    }

    this->lods.Resize(lodCount);
    this->lods.Realloc(this->lods.nElem);

    // Pass 2: assign each template LOD object to its bucket.
    int nDef = pTpl->lodDef.nElem;
    for (int di = 0; di < nDef; ++di)
    {
        animINST *pInst = this->pInst;
        tplLOD_DEF *def = &pLodDef->pData[di];
        int objId = def->objId;
        if (objId < this->pInst->nObj)
        {
            objOBJ *pObj = pInst->ppObjList[objId];
            if (pObj && pObj->pGeom) // decompiler saw ppObjList entry as psSECTION_DATA*; pOwner == objOBJ::pGeom
            {
                this->lods[def->index].objs.PushBack(pObj);
                pObj->stateProc |= 1u; // decompiler: psParents.__cl.line == objOBJ::stateProc (starts hidden)
                if (def->isLastLod)
                {
                    for (int higher = def->index + 1; higher < lodCount; ++higher)
                        this->lods[higher].objs.PushBack(pObj);
                }
            }
        }
        nDef = pTpl->lodDef.nElem;
    }

    // Pass 3: shrink each bucket's object vector to its exact size.
    for (int b = 0; b < this->lods.nElem; ++b)
        this->lods.pData[b].objs.Realloc(this->lods.pData[b].objs.nElem);

    // Reset the distance list (count only) and reserve room for one distance per bucket.
    this->lodDist.nElem = 0;
    if (lodCount > this->lodDist.allocated)
        this->lodDist.Realloc(lodCount);

    // Resolve the instance ps "LOD" section.
    psSECTION_INTERFACE lodSection;
    {
        psSECTION instPS = this->pInst->GetPS();
        static_cast<psSECTION &>(lodSection) =
            reinterpret_cast<psSECTION_INTERFACE &>(instPS).GetSection("LOD", 0);
    }

    psARRAY distArray = lodSection.GetArray("dist", 0);
    if (distArray.Length() <= 0)
    {
        // No explicit distances: derive them from maxDist and a gamma power curve.
        float maxDist = 100.0f;
        float gamma = 1.5f;
        lodSection.GetFloat("maxDist", &maxDist, 0);
        lodSection.GetFloat("gamma", &gamma, 0);

        int denom = lodCount - 1;
        for (int level = 0; level < denom; ++level)
        {
            float fraction = (float)(level + 1) / (float)denom;
            _m3dCheckValid(fraction);
            _m3dCheckValid(gamma);
            float dist = (float)pow((double)fraction, (double)gamma) * maxDist;
            this->lodDist.PushBack(dist);
        }
        if (lodCount > 0)
            this->lodDist.PushBack(M3D_INFINITY);
        this->lodDist.Realloc(this->lodDist.nElem);
    }
    else
    {
        // Explicit distances from the "dist" array (clamped to the bucket count).
        int nUse = (lodCount >= distArray.Length()) ? distArray.Length() : lodCount;
        for (int idx = 0; idx < nUse; ++idx)
        {
            float dist = distArray.GetFloat(idx, 0.0f);
            this->lodDist.PushBack(dist);
        }

        if (lodCount - distArray.Length() > 1)
        {
            const char *instName = this->pInst->name;
            if (!instName)
                instName = "";
            _apLog("~Error,LOD~[%s:%s] Too few lod distances", instName, this->pInst->pTpl->name);
        }

        // Pad any remaining buckets with the far sentinel.
        int nPad = lodCount - distArray.Length();
        while (nPad > 0)
        {
            this->lodDist.PushBack(M3D_INFINITY);
            --nPad;
        }
    }

    this->transZone = 0.0f;
    lodSection.GetFloat("distLODTrans", &this->transZone, 0);
    this->lodNmbCur = -1;
    this->SetLodNmb(0, 0.0f);
}
