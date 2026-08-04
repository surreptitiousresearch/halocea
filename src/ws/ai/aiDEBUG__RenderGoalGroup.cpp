// ws-engine ai08 (a8_debug.cpp): aiDEBUG::RenderGoalGroup @0x83198998 (protected).
// Draw the goal/path debug gizmos for this brain: the dynamic-nav-sampling points, the raw and
// optimized behaviour paths, the high-obstacle path arrows, and the movement goal marker/line
// (plus the goal reach-check). Each block is gated by its own con-var toggle.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h" // rendDrv, dlFree, _m3dCheckValid, con-vars, vectGLColor
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"             // GetTeamID
#include "../../headers/ws/ai/aiTEAMS_SYS.h"               // GetDbgColorMask
#include "../../headers/ws/ai/aiPLANNER.h"                 // aiPLANNER (->teams)
#include "../../headers/ws/ai/aiCON_CB_globals.h"          // aiPlanner
#include "../../headers/ws/ai/aiCMD2BODY.h"                // cmd.goal
#include "../../headers/ws/ai/aiCMD_GOAL.h"                // posGoal / posChkReach
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/apCL.h"

static const char *const kSrcFile =
    "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_debug.cpp";

// NOTE: the cross colours the decompiler left uninitialised are this->Color (the active debug tint,
// carried in from the preceding render-group call — RenderGoalGroup never reassigns it).
void aiDEBUG::RenderGoalGroup()
{
    propGAME_INFO_TEAM *teamID = this->pBrain->GetTeamID();
    unsigned int dbgColorMask =
        ((aiTEAMS_SYS *)&aiPlanner->teams)->GetDbgColorMask(*teamID);

    if (this->pBrain->IsDead())
        return;

    if (dbg_isDNSPoints.value)
    {
        for (int i = 0; i < this->dns.pntCheck.nElem; ++i)
            rendDrv->RenderCross(&this->dns.pntCheck[i], 0.2f, this->Color);
        if (this->dns.points.nElem)
            rendDrv->RenderCross(&this->dns.points[0], 0.40000001f, this->Color);
        for (int j = 1; j < this->dns.points.nElem; ++j)
            rendDrv->RenderCross(&this->dns.points[j], 0.2f, this->Color);
    }

    if (dbg_isPathFromBHV.value)
    {
        apCL cl = { kSrcFile, 1341 };
        dsVECTOR<m3dV, 8> path(cl);
        this->GrabPathPts(path, false);
        for (int i = 0; i < path.nElem - 1; ++i)
            rendDrv->RenderLine(&path[i], &path[i + 1], 0xFFFF00FF);
        dlFree(path.pData);
    }

    if (dbg_isPathOptFromBHV.value)
    {
        apCL cl = { kSrcFile, 1355 };
        dsVECTOR<m3dV, 8> pathOpt(cl);
        this->GrabPathPts(pathOpt, true);
        for (int i = 0; i < pathOpt.nElem - 1; ++i)
        {
            rendDrv->RenderLine(&pathOpt[i], &pathOpt[i + 1], 0xFFFF9FFF);
            rendDrv->RenderCross(&pathOpt[i], 0.050000001f, this->Color);
        }
        dlFree(pathOpt.pData);
    }

    if (dbg_isHOPathFromBHV.value)
    {
        apCL cl = { kSrcFile, 1370 };
        dsVECTOR<m3dV, 8> hoPath(cl);
        float posStart = 0.0f, posEnd = 0.0f;
        this->GrabHOPathPts(hoPath, &posStart, &posEnd);

        int lastIdx = hoPath.nElem - 1;
        if (lastIdx > 0)
        {
            // Points arrive in triples (offset, from, to): draw a two-segment shaft to a pair of
            // control points plus an arrow-tip whose length is scaled by the start/end scalars.
            for (int i = 0; i < lastIdx; i += 3)
            {
                m3dV scaledStart = hoPath[i] * 0.1f;
                m3dV from = hoPath[i + 1];
                m3dV to   = hoPath[i + 2];
                m3dV dir  = to - from;
                m3dNormalize(&dir);
                m3dV dir01 = dir * 0.1f;
                m3dV ctrl1 = scaledStart + from + dir01;
                m3dV ctrl2 = scaledStart + to - dir01;

                unsigned int lastLineColor;
                if (i != 0 || lastIdx > 3)
                {
                    if (i == 0)
                    {
                        m3dV tip = (scaledStart + from) + dir * posStart;
                        rendDrv->RenderLine(&from, &ctrl1, 0xFFFFFF00);
                        rendDrv->RenderLine(&ctrl1, &tip, 0xFFFFFF00);
                        rendDrv->RenderLine(&tip, &ctrl2, 0xFF00FF00);
                        lastLineColor = 0xFF00FF00;
                    }
                    else if (i + 3 < lastIdx)
                    {
                        rendDrv->RenderLine(&from, &ctrl1, 0xFF00FF00);
                        rendDrv->RenderLine(&ctrl1, &ctrl2, 0xFF00FF00);
                        lastLineColor = 0xFF00FF00;
                    }
                    else
                    {
                        m3dV tip = (scaledStart + from) + dir * posEnd;
                        rendDrv->RenderLine(&from, &ctrl1, 0xFF00FF00);
                        rendDrv->RenderLine(&ctrl1, &tip, 0xFF00FF00);
                        rendDrv->RenderLine(&tip, &ctrl2, 0xFFFFFF00);
                        lastLineColor = 0xFFFFFF00;
                    }
                }
                else
                {
                    m3dV tipStart = (scaledStart + from) + dir * posStart;
                    m3dV tipEnd   = (scaledStart + from) + dir * posEnd;
                    rendDrv->RenderLine(&from, &ctrl1, 0xFFFFFF00);
                    rendDrv->RenderLine(&ctrl1, &tipStart, 0xFFFFFF00);
                    rendDrv->RenderLine(&tipStart, &tipEnd, 0xFF00FF00);
                    rendDrv->RenderLine(&tipEnd, &ctrl2, 0xFFFFFF00);
                    lastLineColor = 0xFFFFFF00;
                }
                rendDrv->RenderLine(&ctrl2, &to, lastLineColor);
            }
        }
        dlFree(hoPath.pData);
    }

    if (dbg_isGoal.value)
    {
        m3dV pos = this->pBrain->GetPos();
        float raisedY = pos.y + 0.5f;
        pos.y = raisedY;
        m3dV goal = this->pBrain->cmd.goal.posGoal;
        if (!dbg_isGoalH.value)
            goal.y = raisedY;   // flatten the goal marker to the (raised) brain height
        rendDrv->RenderLine(&pos, &goal, vectGLColor & dbgColorMask);
        rendDrv->RenderCross(&goal, 0.5f, this->Color);
        rendDrv->RenderCross(&pos, 0.2f, this->Color);

        if (dbg_isGoalReach.value)
        {
            m3dV reach = this->pBrain->cmd.goal.posChkReach;
            rendDrv->RenderLine(&pos, &reach, dbgColorMask & 0xFF00FF00);
            rendDrv->RenderCross(&reach, 0.1f, this->Color);
        }
    }
}
