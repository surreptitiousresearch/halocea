// aiCHASER::RequestPathToBestView @ 0x8320C3C0
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/nsSUSCACHE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/nav/navPATH.h"
#include "../../headers/ws/nav/navQUERY.h"
#include "../../headers/apCL.h"
#include "../../headers/ws/m3d/m3dV.h"

static const char *const kSrcFile =
    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_plan_chas.cpp";

bool aiCHASER::RequestPathToBestView(aiBRAIN *pBrain, navPATH &path)
{
    navSYS *sys = pBrain->GetNavSystem();
    nsSUSCACHE *cache = FindCache(sys);
    if (!cache)
        return false;

    cache->ConsiderAddingRandom();

    apCL cl = { kSrcFile, 387 };
    navQUERY query(pBrain, &cl);

    m3dV startPos = pBrain->GetPos();

    m3dV bestView;
    if (!cache->CopyBestView(&query.toGlob, &bestView))
        return false;

    while (true)
    {
        // Minimum path distance: near-zero for small-unit nav-systems, ~4.1m otherwise.
        float minDist = sys->isSmallSize ? 0.000001f : 4.0999999f;
        if (query.toGlob.nElem == 1)
        {
            query.toGlob.nElem = 0;
            query.vTo = &bestView;
        }
        query.minDist = minDist;
        query.applyPenalties = true;
        query.applyHOs = true;
        query.applyUsedPaths = true;
        if (sys->BuildPath(&query, &path))
            break;

        cache->DeleteBest(&query.toGlob);
        query.vTo = nullptr;
        query.toGlob.nElem = 0;
        if (!cache->CopyBestView(&query.toGlob, &bestView))
            return false;
    }

    cache->DeleteNear(&startPos);
    m3dV endPos;
    path.GetEffectivePos(path.pathWP.nElem - 1, &endPos);
    cache->DeleteNear(&endPos);
    return true;
}
