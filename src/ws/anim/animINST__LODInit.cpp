/* ?LODInit@animINST@@QAAXXZ @0x82B0EC98 */
#include "animINST.h"
#include "animLOD_LOD_INFO.h"
#include "objOBJ_boundary.h"
#include "../ps/psSECTION_INTERFACE.h"
#include "../ds/dsAFFIX_READER.h"
#include "anim_boundary.h"
#include <new>

// Debug placement operator new (size, file, line) the ws build links in. boundary.
void *operator new(size_t size, const char *file, unsigned int line);

// ?LODInit@animINST@@QAAXXZ  (0x82B0EC98) — allocate and populate the instance's level-of-detail
// table: build the per-level object lists, seed default switch distances (overridden by the "LOD"
// section and by lod_distance affixes), and mark every LOD object as LOD-managed.
void animINST::LODInit()
{
    if (!this->pObj)
        return;

    animLOD *pLOD = (animLOD *)operator new(0x64u, "D:\\Projects\\code\\common\\src.sys\\objects\\lod.cpp", 0x192u);
    if (pLOD)
        new (pLOD) animLOD(this);
    this->pLOD = pLOD;
    this->pLOD->Init();
    this->FillObjRendList(false);

    // mark every object belonging to a non-final LOD level as LOD-managed
    for (int i = 0; i < this->pLOD->lods.nElem - 1; ++i)
    {
        animLOD::LOD_INFO *pLodInfo = &this->pLOD->lods[i];
        for (int j = 0; j < pLodInfo->objs.nElem; ++j)
            pLodInfo->objs[j]->stateProc |= 0x8000000u;
    }

    // default switch distances (far set when the instance is flagged "big", offset 0x40)
    animLOD *pLODCfg = this->pLOD;
    if ((this->state & 0x40) != 0)
    {
        pLODCfg->distLODAnim = 200.0f;
        this->pLOD->distLODOffMatrModel = 1000.0f;
        this->pLOD->distLODOffAnimBlend = 1000.0f;
        this->pLOD->distLODOffLTAnimBlend = 1000.0f;
        this->pLOD->distLODHideInst = 1000.0f;
        this->pLOD->distLODHideObjInst = 1000.0f;
    }
    else
    {
        pLODCfg->distLODAnim = 50.0f;
        this->pLOD->distLODOffMatrModel = 12.0f;
        this->pLOD->distLODOffAnimBlend = 55.0f;
        this->pLOD->distLODOffLTAnimBlend = 8.0f;
        this->pLOD->distLODHideInst = 20.0f;
        this->pLOD->distLODHideObjInst = 20.0f;
    }

    // per-section overrides from the "LOD" sub-section of the instance parameter section
    psSECTION lodSection = reinterpret_cast<psSECTION_INTERFACE *>(&this->ps)->GetSection("LOD", 0);
    psSECTION_INTERFACE *pLodCfgSection = reinterpret_cast<psSECTION_INTERFACE *>(&lodSection);
    pLodCfgSection->GetFloat("distLODAnim", &this->pLOD->distLODAnim, 0);
    pLodCfgSection->GetFloat("distLODOffMatrModel", &this->pLOD->distLODOffMatrModel, 0);
    pLodCfgSection->GetFloat("distLODOffAnimBlend", &this->pLOD->distLODOffAnimBlend, 0);
    pLodCfgSection->GetFloat("distLODOffLTAnimBlend", &this->pLOD->distLODOffLTAnimBlend, 0);
    pLodCfgSection->GetFloat("distLODHide", &this->pLOD->distLODHideInst, 0);
    pLodCfgSection->GetFloat("distLODHideObj", &this->pLOD->distLODHideObjInst, 0);
    pLodCfgSection->GetFloat("distLODPauseAnim", &this->pLOD->distLODPauseAnim, 0);

    // enforce a minimum of 25 units on the two blend-off distances
    animLOD *pLODClampM = this->pLOD;
    float matrModelDist = pLODClampM->distLODOffMatrModel;
    if (matrModelDist <= 25.0f)
        matrModelDist = 25.0f;
    pLODClampM->distLODOffMatrModel = matrModelDist;

    animLOD *pLODClampB = this->pLOD;
    float animBlendDist = 25.0f;
    if (pLODClampB->distLODOffAnimBlend > 25.0f)
        animBlendDist = pLODClampB->distLODOffAnimBlend;
    pLODClampB->distLODOffAnimBlend = animBlendDist;

    // affix override: "lod_distance_vis" flags the instance and can set the hide distance
    dsAFFIX_READER reader(this->affixes.str); // ctor takes const dsTSTRING<char>& (affixes.str)
    dsTSTRING_BUF_HEADER<char> *pAffixBuffer = reader.affixStr.pBuffer;
    if (reader.affixStr.pBuffer->strLen)
    {
        if (reader.HaveAffix("lod_distance_vis", 0))
        {
            this->state2 |= 0x8000u;
            float distance[3];
            if (reader.GetFloat("distance", distance))
                this->pLOD->distLODHideInst = distance[0];
        }
        pAffixBuffer = reader.affixStr.pBuffer;
    }
    // inlined dsAFFIX_READER destructor: release its affix-string buffer
    if (!--pAffixBuffer->refCount)
        dlFree(pAffixBuffer);
    // lodSection's psSECTION destructor runs at scope exit
}
