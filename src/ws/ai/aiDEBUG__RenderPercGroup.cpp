// aiDEBUG::RenderPercGroup @ 0x8319B220
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h"  // rendDrv, RenderPolyline, operators, dlFree
#include "../../headers/ws/ai/aiDEBUG_render2_boundaries.h" // dbg_is* perception con-vars
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"              // GetTeamID / GetPos
#include "../../headers/ws/ai/aiBRAIN_DESC.h"               // aiBRAIN_DESC::TYPE_ID
#include "../../headers/ws/ai/aiSTATUS.h"                   // st->mind.val
#include "../../headers/ws/ai/aiMIND.h"                     // mind.vision
#include "../../headers/ws/ai/aiVISION_BASE.h"              // IsReal
#include "../../headers/ws/ai/aiBODY_IFACE.h"               // DbgGetMatrShapeCdt
#include "../../headers/ws/ai/aiTEAMS_SYS.h"                // GetDbgColorMask
#include "../../headers/ws/ai/aiPLANNER.h"                  // aiPLANNER (->teams)
#include "../../headers/ws/ai/aiCON_CB_globals.h"           // aiPlanner
#include "../../headers/ws/mdl/mdlCDT_SETTINGS.h"           // GetCdtParams()->radCdt
#include "../../headers/ws/msg/msgSYSTEM.h"                 // msgSystem, FindUID, msgADDR
#include "../../headers/ws/ds/dsTYPE_ID.h"                  // IsDerivedFrom
#include "../../headers/animINST.h"                         // GetOBB / GetPos
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3dSPHERE.h"
#include "../../headers/ws/m3d/m3dOBB.h"

// Draw the whole per-brain perception debug group, gated by per-feature con-vars: sight cone,
// cheap-vision glyph, hearing, vision probes, find-enemy probes, vision-cylinder slice, line-of-fire
// shapes, possible-enemy links, and radar. Skipped entirely when the brain is dead.
void aiDEBUG::RenderPercGroup()
{
    if (this->pBrain->IsDead())
        return;

    if (dbg_isSight.value)
        RenderSight();

    if (dbg_isVisionCheap.value)
    {
        aiSTATUS *st = this->pBrain->st.pointee;
        // Glyph is anchored 2.5 units above the body along its up axis.
        m3dV base  = st->cs.posBody + (2.5f * st->cs.dirUpBody);
        m3dV up    = st->cs.dirUpBody;
        m3dV right = st->cs.dirRightBody;

        dsVECTOR<m3dV, 8> poly;
        bool visionReal = this->pBrain->mind.vision.pointee->IsReal();

        // DEVIATION: the per-vertex operand vectors of the cheap-vision "eye" glyph were lost by the
        // decompiler (reused return registers r3/r4). The stroke count, scalar factors and colours
        // below are exact; the up/right decomposition of each vertex is reconstructed structurally.
        if (visionReal)
        {
            poly.PushBack(base - right * 0.1f);
            poly.PushBack((base - right * 1.0f) + up * 0.25f);
            poly.PushBack((base - right * 0.2f) + up * 0.2f);
            poly.PushBack(poly.Front());
            RenderPolyline(&poly, 0xFF00FF00);

            poly.nElem = 0;
            poly.PushBack((base - right * 0.13f) + up * 0.2f);
            poly.PushBack((base - right * 0.75f) + up * 0.30000001f);
            RenderPolyline(&poly, 0xFF00FF00);

            poly.nElem = 0;
            poly.PushBack(base + right * 0.1f);
            poly.PushBack((base + right * 1.0f) + up * 0.25f);
            poly.PushBack((base + right * 0.2f) + up * 0.2f);
            poly.PushBack(poly.Front());
            RenderPolyline(&poly, 0xFF00FF00);

            poly.nElem = 0;
            poly.PushBack((base + right * 0.13f) + up * 0.2f);
            poly.PushBack((base + right * 0.75f) + up * 0.30000001f);
            RenderPolyline(&poly, 0xFF00FF00);
        }
        else
        {
            poly.PushBack(base - right * 0.5f);
            poly.PushBack((base + right * 1.0f) + up * 0.5f);
            RenderPolyline(&poly, 0xFFFF0000);

            poly.nElem = 0;
            poly.PushBack(base + right * 0.5f);
            poly.PushBack((base - right * 1.0f) + up * 0.5f);
            RenderPolyline(&poly, 0xFFFF0000);
        }
    }

    if (dbg_isHearing.value)
        RenderHearing();

    if (dbg_isVision.value)
    {
        for (int i = 0; i < this->vision.lines.nElem; ++i)
        {
            aiDEBUG::m3dVPair &pr = this->vision.lines[i];
            rendDrv->RenderLine(&pr.posFrom, &pr.posTo, 0xFF00FF00);
            // DEVIATION: cross colour register lost by the decompiler; use the active debug tint.
            rendDrv->RenderCross(&pr.posTo, 0.050000001f, this->Color);
        }
        for (int j = 0; j < this->vision.posCollisions.nEntry; ++j)
            rendDrv->RenderCross(&this->vision.posCollisions[j], 0.5f, this->Color);
    }

    if (dbg_isFindEnemy.value)
    {
        for (int k = 0; k < this->find_enemy.lines.nElem; ++k)
        {
            aiDEBUG::m3dVPair &pr = this->find_enemy.lines[k];
            rendDrv->RenderLine(&pr.posFrom, &pr.posTo, 0xFF0000FF);
            rendDrv->RenderCross(&pr.posTo, 0.050000001f, this->Color);
        }
    }

    if (dbg_isVisionCylSlice.value)
    {
        // Draw the 10-segment vision cylinder slice as a closed ring.
        for (int i = 0; i < 10; ++i)
            rendDrv->RenderLine(&this->visionCylSlice[i], &this->visionCylSlice[(i + 1) % 10],
                                this->pBrain->dbg.Color);
    }

    if (dbg_isLOF.value)
    {
        aiSTATUS *st = this->pBrain->st.pointee;
        if (((unsigned int)st->mind.val >> 5) & 1)
        {
            m3dMATR shapeCdt;
            this->pBrain->body.pointee->DbgGetMatrShapeCdt(shapeCdt);
            m3dSPHERE sphere;
            shapeCdt.GetOrigin(&sphere.org);
            sphere.radius = this->pBrain->GetCdtParams()->radCdt;
            rendDrv->RenderSphere(&sphere, 0xFFCC0000);
        }
        if (((unsigned int)st->mind.val >> 6) & 1)
        {
            m3dOBB *obb = this->pBrain->pInst->GetOBB(0);
            rendDrv->RenderOBB(obb, 0xFFCCFF00);
        }
    }

    if (dbg_isPOS_ENEMIES.value)
    {
        propGAME_INFO_TEAM *teamID = this->pBrain->GetTeamID();
        unsigned int dbgColorMask =
            ((aiTEAMS_SYS *)&aiPlanner->teams)->GetDbgColorMask(*teamID);

        for (int idx = 0; idx < 32; ++idx)
        {
            unsigned int uid = this->possibleEnemiesUID[idx];
            if (uid == 0xFFFFFFFFu)
                break;
            msgADDR *addr = msgSystem.FindUID(uid);
            if (!addr)
                continue;

            m3dV enemyPos;
            dsTYPE_ID typeId;
            dsTYPE_ID *tid = addr->__vftable->TypeID(addr, &typeId);
            if (tid->IsDerivedFrom(aiBRAIN_DESC::TYPE_ID))
            {
                // Enemy is a brain — fetch its world position through the actor GetPos virtual.
                enemyPos = reinterpret_cast<aiBRAIN_IFACE *>(addr)->GetPos();
            }
            else
            {
                // DEVIATION: non-brain endpoint — the decompiler reads the entity's animINST pointer
                // as msgADDR[10].priorityInt (raw actor offset 0x15C); kept as raw access.
                animINST *inst = *(animINST **)((char *)addr + 0x15C);
                if (inst)
                    inst->GetPos(&enemyPos);
            }

            m3dV brainPos = this->pBrain->GetPos();
            this->Color = dbgColorMask & 0xFF770055;
            rendDrv->RenderLine(&brainPos, &enemyPos, this->Color);
            rendDrv->RenderCross(&enemyPos, 0.30000001f, this->Color);
        }
    }

    if (dbg_isRadar.value)
    {
        this->Color = 0xFF770055; // -8978347
        rendDrv->RenderLine(&this->vRadarFrom, &this->vRadarTo, 0xFF770055);
        rendDrv->RenderCross(&this->vRadarTo, 0.30000001f, this->Color);
        rendDrv->RenderCross(&this->vRadarFrom, 0.2f, this->Color);
        if (((this->state.state >> 2) & 1) != 0)
            rendDrv->RenderCross(&this->vRadarHit, 0.40000001f, this->Color);
    }
}
