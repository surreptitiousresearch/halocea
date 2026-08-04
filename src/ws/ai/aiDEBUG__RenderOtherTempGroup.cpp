#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h" // rendDrv, dbg vars, bhvBEHAV
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"             // GetTeamID
#include "../../headers/ws/ai/aiTEAMS_SYS.h"               // GetDbgColorMask
#include "../../headers/ws/ai/aiPLANNER.h"                 // aiPLANNER (->teams)
#include "../../headers/ws/ai/aiCON_CB_globals.h"          // aiPlanner
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"               // GetMajorBehavior
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiDEBUG::RenderOtherTempGroup @ 0x83199558  (?RenderOtherTempGroup@aiDEBUG@@IAAXXZ)
//
// Draw the miscellaneous "temp" debug groups: the vision-cylinder slice fan, the best-behaviour
// debug line, the interpolated temp-line/temp-position sets, and the IK position markers.
//
// NOTE: the decompiler expressed the fan's vCheckFrom/vCheckTo arrays as raw pBrain-relative byte
// offsets (0x33C.. / propContainer.typeId+i); those resolve exactly to this->vCheckFrom[k] /
// this->vCheckTo[k] (aiDEBUG lives at aiBRAIN+0x324). Reconstructed via the named members. The
// per-call cross colours the decompiler left uninitialised are this->Color (the active debug tint).
void aiDEBUG::RenderOtherTempGroup()
{
    if (dbg_isFan.value)
    {
        for (int k = 0; k < 10; ++k)
        {
            rendDrv->RenderLine(&this->vCheckFrom[k], &this->vCheckTo[k], this->Color);
            rendDrv->RenderCross(&this->vCheckTo[k], 0.5f, this->Color);
            rendDrv->RenderCross(&this->vCheckFrom[k], 0.2f, this->Color);
        }
        rendDrv->RenderLine(&this->vCylFrom, &this->vCylTo, 0xFFFFFF00);
        rendDrv->RenderCross(&this->vCylFrom, 0.2f, this->Color);
        rendDrv->RenderCross(&this->vCylTo, 0.2f, this->Color);
    }

    propGAME_INFO_TEAM *teamID = this->pBrain->GetTeamID();
    unsigned int dbgColorMask =
        ((aiTEAMS_SYS *)&aiPlanner->teams)->GetDbgColorMask(*teamID);
    this->Color = dbgColorMask & 0xFFBB0000;

    if (dbg_isBhvLine.value)
    {
        bhvBEHAV *bhv = this->pBrain->mind.best.pointee->GetMajorBehavior(0);
        if (bhv)
        {
            m3dV from, to;
            if (bhv->GetRenderDebug(&to, &from, &this->Color))
            {
                rendDrv->RenderLine(&to, &from, this->Color);
                rendDrv->RenderCross(&from, 0.5f, this->Color);
                rendDrv->RenderCross(&to, 0.2f, this->Color);
            }
        }
    }

    if (dbg_isOtherTempLn.value)
    {
        int count = this->otherTempLinesCount;
        if (count > 0)
        {
            if ((unsigned int)count > 100)
                count = 100;
            this->otherTempLinesCount = count;
            for (int k = 0; k < count; ++k)
            {
                unsigned long color = this->InterpolateColor(0xFF00FFFF, 0x7F00, count, k);
                rendDrv->RenderLine(&this->otherTempLinesFrom[k], &this->otherTempLinesTo[k], color);
            }
        }
    }

    if (dbg_isOtherTempPos.value)
    {
        int count = this->otherTempPosCount;
        if (count > 0)
        {
            if ((unsigned int)count > 100)
                count = 100;
            this->otherTempPosCount = count;
            for (int k = 0; k < count; ++k)
            {
                unsigned long color = this->InterpolateColor(0xFFFF0000, 0xFF, count, k);
                rendDrv->RenderCross(&this->otherTempPos[k], 0.2f, color);
            }
        }
    }

    if (dbg_isIK.value)
    {
        for (int j = 0; j < this->posList.nElem; ++j)
        {
            aiDEBUG::POS_VIEW &pv = this->posList[j];
            rendDrv->RenderCross(&pv.pos, pv.size, this->Color);
        }
    }
}
