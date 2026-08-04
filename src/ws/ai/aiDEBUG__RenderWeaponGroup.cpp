#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h" // rendDrv, dbg vars, _isGrenDbgLine
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"             // GetTeamID
#include "../../headers/ws/ai/aiTEAMS_SYS.h"               // GetDbgColorMask
#include "../../headers/ws/ai/aiPLANNER.h"                 // aiPLANNER (->teams)
#include "../../headers/ws/ai/aiCON_CB_globals.h"          // aiPlanner
#include "../../headers/ws/ai/aiBODY_IFACE.h"              // body->wpn
#include "../../headers/ws/ai/aiWEAPON_BASE.h"             // GetPosDirTip / IsAbsent
#include "../../headers/ws/ai/aiSTATUS.h"                  // st->wpn flags
#include "../../headers/ws/ai/aiCMD2BODY.h"                // cmd.posAim/posLookAt/posWpnTgt
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/m3d/m3dV.h"
#include <math.h>

// aiDEBUG::RenderWeaponGroup @ 0x83198178  (?RenderWeaponGroup@aiDEBUG@@IAAXXZ)
//
// Draw the weapon / aiming / trajectory debug visualisation for a live brain: grenade markers, the
// grenade-avoidance CDT probes, the basic-aiming points, the aim/look-at rays, the parabolic and
// linear+insertion trajectory arcs, and the weapon-target line. Each block is gated by its con-var.
//
// NOTE: the per-call cross colours the decompiler left uninitialised are this->Color (the active
// debug tint, re-assigned per block from the team debug-colour mask).
void aiDEBUG::RenderWeaponGroup()
{
    if (this->pBrain->IsDead())
        return;

    for (int i = 0; i < this->grenades.nElem; ++i)
    {
        aiDEBUG::m3dVPair &g = this->grenades[i];
        rendDrv->RenderCross(&g.posFrom, 0.2f, this->Color);
        rendDrv->RenderCross(&g.posTo, 0.2f, this->Color);
        rendDrv->RenderLine(&g.posFrom, &g.posTo, 0xFF00FF00);
    }

    propGAME_INFO_TEAM *teamID = this->pBrain->GetTeamID();
    unsigned int dbgColorMask =
        ((aiTEAMS_SYS *)&aiPlanner->teams)->GetDbgColorMask(*teamID);

    if (_isGrenDbgLine)
    {
        rendDrv->RenderCross(&this->grenadeAvoidLine.posClosestToBrain, 0.2f, this->Color);
        for (int j = 0; j < this->CDTtest_ends.nElem; ++j)
        {
            rendDrv->RenderLine(&this->grenadeAvoidLine.CDTtest_start, &this->CDTtest_ends[j], 0xFF0000FF);
            rendDrv->RenderCross(&this->CDTtest_fails[j], 0.2f, this->Color);
        }
        const m3dV &succ = this->grenadeAvoidLine.CDTtest_success;
        if (fabsf(succ.x - m3dVZero.x) >= 0.000001f ||
            fabsf(succ.y - m3dVZero.y) >= 0.000001f ||
            fabsf(succ.z - m3dVZero.z) >= 0.000001f)
        {
            rendDrv->RenderLine(&this->grenadeAvoidLine.CDTtest_start, &succ, 0xFF00FF00);
        }
    }

    if (dbg_isAimingPoints.value)
    {
        rendDrv->RenderCross(&this->posBasicAiming, 0.30000001f, this->Color);
        rendDrv->RenderCross(&this->posPrevBasicAiming, 0.2f, this->Color);
        rendDrv->RenderLine(&this->posPrevBasicAiming, &this->posBasicAiming, 0xFF00FF00);
        this->Color = dbgColorMask & 0xFF558800;
        rendDrv->RenderCross(&this->pointForProjectile, 0.69999999f, this->Color);

        aiWEAPON_BASE *wpn = this->pBrain->body.pointee->wpn.pointee;
        m3dV tip, dir;
        wpn->GetPosDirTip(&tip, &dir);
        rendDrv->RenderCross(&tip, 0.30000001f, this->Color);
        _m3dCheckValid(&dir);
        m3dV dir3 = { dir.x * 3.0f, dir.y * 3.0f, dir.z * 3.0f };
        _m3dCheckValid(&tip);
        _m3dCheckValid(&dir3);
        m3dV tipEnd = { tip.x + dir3.x, tip.y + dir3.y, tip.z + dir3.z };
        rendDrv->RenderLine(&tip, &tipEnd, 0xFF000099);
    }

    if (dbg_isAim.value)
    {
        m3dV org = this->vOrgFace;
        m3dV aim = this->pBrain->cmd.posAim;
        this->Color = dbgColorMask & 0xFFBB00EE;
        rendDrv->RenderLine(&org, &aim, this->Color);
        rendDrv->RenderCross(&aim, 0.5f, this->Color);
        rendDrv->RenderCross(&org, 0.2f, this->Color);
    }

    if (dbg_isLookAt.value)
    {
        m3dV org = this->vOrgFace;
        m3dV look = this->pBrain->cmd.posLookAt;
        this->Color = dbgColorMask & 0xFFBB88EE;
        rendDrv->RenderLine(&org, &look, this->Color);
        rendDrv->RenderCross(&look, 0.5f, this->Color);
        rendDrv->RenderCross(&org, 0.2f, this->Color);
    }

    if (dbg_isTrajectory.value)
    {
        int last = this->traj.parabPoints.nEntry - 1;
        for (int k = 1; k < last; ++k)
            rendDrv->RenderLine(&this->traj.parabPoints[k - 1], &this->traj.parabPoints[k],
                                dbgColorMask & 0xFF0000FF);
        for (int k = 0; k < last; ++k)
            rendDrv->RenderCross(&this->traj.parabPoints[k], 0.1f, this->Color);
        if (this->traj.hasParabIntersec)
            rendDrv->RenderCross(&this->traj.intersecParab, 1.0f, this->Color);

        this->Color = dbgColorMask & 0xFF000033;
        rendDrv->RenderCross(&this->traj.linearFrom, 0.1f, this->Color);
        rendDrv->RenderCross(&this->traj.linearMiddle, 0.1f, this->Color);
        rendDrv->RenderCross(&this->traj.linearTo, 0.1f, this->Color);
        rendDrv->RenderLine(&this->traj.linearFrom, &this->traj.linearTo, this->Color);
        if (this->traj.hasLinearIntersec)
            rendDrv->RenderCross(&this->traj.linearObst, 1.0f, this->Color);
    }

    if (dbg_isManipTraj.value)
    {
        this->Color = (this->traj.hasInsIntersec ? 0xFFAA0000u : 0xFF0000AAu) & dbgColorMask;
        rendDrv->RenderCross(&this->traj.insFrom, 0.1f, this->Color);
        rendDrv->RenderCross(&this->traj.insMiddle, 0.1f, this->Color);
        rendDrv->RenderCross(&this->traj.insTo, 0.1f, this->Color);
        rendDrv->RenderLine(&this->traj.insFrom, &this->traj.insTo, this->Color);
        if (this->traj.hasInsIntersec)
            rendDrv->RenderCross(&this->traj.insObst, 1.0f, this->Color);
    }

    if (dbg_isWpnTgt.value)
    {
        aiWEAPON_BASE *wpn = this->pBrain->body.pointee->wpn.pointee;
        if (!wpn->IsAbsent())
        {
            aiWEAPON_BASE *wpn2 = this->pBrain->body.pointee->wpn.pointee;
            m3dV tip, dir;
            wpn2->GetPosDirTip(&tip, &dir);
            m3dV passed = this->posPassedWpnTgt;
            m3dV target = this->pBrain->cmd.posWpnTgt;

            unsigned int color =
                ((this->pBrain->st.pointee->wpn.val >> 1) & 1) ? 0xFFBB0B0Bu : 0xFF0B0BBBu;
            this->Color = color & dbgColorMask;
            rendDrv->RenderLine(&tip, &passed, this->Color);
            rendDrv->RenderCross(&target, 0.2f, this->Color);
            rendDrv->RenderCross(&tip, 0.1f, this->Color);
            this->Color = dbgColorMask & 0xFFEE1111;
            rendDrv->RenderLine(&target, &passed, this->Color);
            rendDrv->RenderCross(&passed, 0.1f, this->Color);
        }
    }
}
