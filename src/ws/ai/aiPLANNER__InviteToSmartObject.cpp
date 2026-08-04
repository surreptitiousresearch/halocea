#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/smt/smtSMT_ENTER_INFO.h"
#include "../../headers/ws/ai/aiBRAIN_PATH.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/nav/navQUERY.h"
#include "../../headers/ws/nav/navPATH.h"
#include "../../headers/ws/bhp/bhpMAJOR_SMART_PAR.h"
#include "../../headers/apCL.h"
#include <string.h>

// ---- boundary externs (deep smt/nav/bhp/brain leaves — bounded-descent frontier) --------------
extern void  _apLog(const char *fmt, ...);
extern void dlFree(void *ptr);
extern const m3dV m3dVZero;
// Fill `out` with the world position of a smart-object entry point (entTRACKER::GetPos on
// ep->placeEnter->trk). boundary.
extern void  smtEnterInfoGetPos(const smtSMT_ENTER_INFO *ep, m3dV *out);
// Nearest enabled waypoint on `ns` within `radius` of `pos`; writes its id to *outWpid. boundary.
extern bool  GetClosestWPtoPos(navSYS *ns, const m3dV *pos, float radius, short *outWpid);
extern navSYS *aiBRAIN_GetNavSystem(aiBRAIN *ai);          // ai->GetNavSystem() virtual. boundary
extern const m3dV *aiBRAIN_BodyPos(aiBRAIN *ai);           // ai->st.pointee->cs.posBody. boundary
// ai->mind.best.pointee->SetBehaviorByParams(par, name): returns true on success. boundary.
extern bool  aiBRAIN_SetSmartBehavior(aiBRAIN *ai, bhpMAJOR_SMART_PAR *par, const char *name);
// aiTEAMS_SYS::CanInvitePlayerToSmt(team) on this planner's teams subsystem. boundary.
extern bool  aiTEAMS_CanInvitePlayerToSmt(const propGAME_INFO_TEAM &team);
// dsSPrintf(dst, fmt, ...): formatted build into a dsTSTRING (sret pointer return). boundary.
extern dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *dst, const char *fmt, ...);

static const char *const kSrcFile =
    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_planner.cpp";

// aiPLANNER::InviteToSmartObject(...) — 0x83168B40
// Gather eligible AI units (and count eligible players) on `team`, then for each smart-object
// entry point try to build the shortest nav-path from an as-yet-unassigned candidate, assigning
// the winning candidate a "major smart" behaviour that walks it into the object. Returns true when
// every path was built and every behaviour accepted; false if there are too few candidates/paths
// or any behaviour is rejected.
//
// NOTE: the PPC decompiler flagged local-allocation failure on this function and packed several
// reference args into __int64 halves; arg order/const here follow the demangled signature. The
// second (final) nav query is built against the winning candidate's brain (best.ai) — the
// decompiler showed a reused file-string register there, corrected as a documented deviation.
bool aiPLANNER::InviteToSmartObject(const entENTITY *caller,
                                    dsSHARED_PTR<smtSMART_OBJ_BASE, 0, Deleter<smtSMART_OBJ_BASE> > &smtObj,
                                    const dsVECTOR<smtSMT_ENTER_INFO, 8> &entryPoints,
                                    const propGAME_INFO_TEAM &team, bool canInviteAI, bool canInvitePlayer)
{
    dsVECTOR<aiBRAIN *, 8> candidates;
    memset(&candidates, 0, 12); // pData/nElem/allocated = 0

    int nCandidateAI = 0;
    if (canInviteAI)
    {
        int nUnits = wah.arrUnits.nElem;
        for (int i = 0; i < nUnits; ++i)
        {
            aiWATCHER_BRAIN *w = wah.PeekActive(i);
            if (!w)
                continue;
            if (w->Is(caller))
                continue;
            if (w->GetTeamID()->team.id == team.team.id && w->GetNavSystem() && !w->IsInSmartBHV())
            {
                aiBRAIN *b = w->brain.pHandle ? w->brain.pHandle->pPtr : nullptr;
                candidates.PushBack(b);
            }
        }
        nCandidateAI = candidates.nElem;
    }

    int nCandidatePlayers = 0;
    if (canInvitePlayer)
    {
        int nPlayers = wah.arrPlayers.nElem;
        for (int i = 0; i < nPlayers; ++i)
        {
            if (aiTEAMS_CanInvitePlayerToSmt(team) && nCandidateAI < entryPoints.nElem)
                ++nCandidatePlayers;
        }
    }

    int needed = entryPoints.nElem;
    if (nCandidatePlayers + nCandidateAI < needed)
    {
        _apLog("~AI,Error~: planner found only %d candidates when %d needed.",
               nCandidatePlayers + nCandidateAI, needed);
        dlFree(candidates.pData);
        return false;
    }

    // --- Phase 1: assign the shortest reachable candidate to each entry point ------------------
    dsVECTOR<aiBRAIN_PATH, 8> builtPaths;
    memset(&builtPaths, 0, 12);

    apCL clFrom = { kSrcFile, 271 };
    apCL clTo   = { kSrcFile, 317 };

    for (int ep = 0; ep < needed; ++ep)
    {
        const smtSMT_ENTER_INFO &info = entryPoints[ep];

        m3dV enterPos = m3dVZero;
        smtEnterInfoGetPos(&info, &enterPos);

        aiBRAIN_PATH best;
        best.ai = nullptr;
        float bestLen = 888888.0f;
        int   bestIdx = -1;
        bool  haveWinner = false;

        for (int c = 0; c < nCandidateAI; ++c)
        {
            aiBRAIN *ai = candidates[c];
            navSYS  *ns = aiBRAIN_GetNavSystem(ai);

            aiBRAIN_PATH cand;
            cand.ai = ai;
            cand.entryPointInfo = info.info;
            cand.path.Clear();

            navQUERY q(ai, &clFrom);

            short wpFrom, wpTo;
            if (GetClosestWPtoPos(ns, aiBRAIN_BodyPos(ai), 10.0f, &wpFrom) &&
                GetClosestWPtoPos(ns, &enterPos, 10.0f, &wpTo))
            {
                q.toGlob.PushBack(wpTo);
                if (q.wpidFrom == q.toGlob.pData[0])
                {
                    // Candidate already stands at the destination waypoint — take it outright.
                    bestIdx = c;
                    best.ai = cand.ai;
                    // adopt cand.path (empty here) and entry info
                    if (cand.path.pathWP.pData != best.path.pathWP.pData)
                    {
                        best.path.pathWP.Clear();
                        if (best.path.pathWP.allocated != cand.path.pathWP.nElem)
                            best.path.pathWP.Realloc(best.path.pathWP.nElem);
                        best.path.pathWP.Insert(0, cand.path.pathWP.pData, cand.path.pathWP.nElem);
                    }
                    best.path.distReach = cand.path.distReach;
                    best.entryPointInfo = cand.entryPointInfo;
                    haveWinner = true;
                    break;
                }

                q.applyPenalties   = true;
                q.applyHOs         = true;
                q.applyUsedPaths   = true;
                q.isAssocCoinciding = false;
                if (ns->BuildPath(&q, &cand.path) &&
                    cand.path.LengthMeters(0, nullptr) < bestLen)
                {
                    bestIdx = c;
                    best = cand;
                    bestLen = cand.path.LengthMeters(0, nullptr);
                }
            }
        }

        if (!haveWinner && bestLen > 888880.0f)
            continue; // no candidate could reach this entry point

        // Build the definitive path (into best.path) for the winner and record it.
        best.path.Clear();
        navQUERY q2(best.ai, &clTo);
        q2.applyUsedPaths   = true;
        q2.applyPenalties   = true;
        q2.isAssocCoinciding = false;
        q2.vTo              = &enterPos;
        q2.applyHOs         = true;

        navSYS *ns2 = aiBRAIN_GetNavSystem(best.ai);
        if (!ns2 || !ns2->BuildPath(ns2 ? &q2 : nullptr, &best.path))
            break;

        builtPaths.PushBack(best);

        // Remove the chosen candidate (unordered) so it is not reused for another entry point.
        memmove(&candidates.pData[bestIdx], &candidates.pData[bestIdx + 1],
                4 * (nCandidateAI - bestIdx - 1));
        candidates.nElem = --nCandidateAI;
    }

    // --- Phase 2: hand each built path to its brain as a "major smart" behaviour ---------------
    int nPaths = builtPaths.nElem;
    if (nCandidatePlayers + nPaths < needed)
    {
        _apLog("~AI,Error~: planner built only %d paths when %d needed.",
               nCandidatePlayers + nPaths, needed);
        for (int p = 0; p < nPaths; ++p)
            builtPaths[p].path.~navPATH();
        dlFree(builtPaths.pData);
        dlFree(candidates.pData);
        return false;
    }

    bool allAccepted = true;
    for (int p = 0; p < nPaths; ++p)
    {
        aiBRAIN_PATH &bp = builtPaths[p];
        aiBRAIN      *ai = bp.ai;

        bhpMAJOR_SMART_PAR par(&smtObj, &bp.entryPointInfo);
        par.isInvited = true;
        par.isDelayedAttachAllowed = true;

        dsTSTRING<char> callerName = caller->GetName();
        dsTSTRING<char> bhvName;
        bhvName.pBuffer = nullptr;
        dsSPrintf(&bhvName, "@invited_by_%s@", callerName.pBuffer->str);
        if (--callerName.pBuffer->refCount == 0)
            dlFree(callerName.pBuffer);

        bool rejected = !aiBRAIN_SetSmartBehavior(ai, &par, bhvName.pBuffer->str);
        if (--bhvName.pBuffer->refCount == 0)
            dlFree(bhvName.pBuffer);

        par.~bhpMAJOR_SMART_PAR();

        if (rejected)
        {
            allAccepted = false;
            break;
        }
    }

    for (int p = 0; p < nPaths; ++p)
        builtPaths[p].path.~navPATH();
    dlFree(builtPaths.pData);
    dlFree(candidates.pData);
    return allAccepted;
}
