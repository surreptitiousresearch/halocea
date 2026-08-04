// aiBRAIN::implTransport @ 0x832B0B48
// ?implTransport@aiBRAIN@@UAAXABV?$dsTSTRING@D@@_N@Z  (virtual)
//
// SSL "Transport(name)" body: teleport the brain's body onto the waypoint or tracked object named
// `nameTrk` (using its orientation). Resolves a world matrix for the target (a nav waypoint if one
// exists, else an entity tracker), snaps the body to it, then re-localizes to the nav system and
// finishes every path / HO / smart / turret behaviour (smart/turret kept only when the body's
// KEEP_BHV_SMART property is set). Rejected by the common SSL back-off gate. `isUseOrient` is unused
// by the body here (the full pose is always applied).
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_boundary.h"   // _apLog
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h"   // aiPlanner
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h" // AI_BID_* enum
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navWP.h"
#include "../../headers/ws/nav/wpWP.h"
#include "../../headers/ws/ent/entTRACKER.h"
#include "../../headers/ws/fsm/fsmCB_POS_DIR_PAR.h"
#include "../../headers/ws/ctrl/ctrlPROP_LIST.h"
#include "../../headers/ws/ctrl/ctrlPROP_ID.h"
#include "../../headers/ws/ds/dsBIT_ARRAY_IMPL.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsFLAGS.h"
#include "../../headers/ws/m3d/m3dMATR.h"
#include "../../headers/ws/m3d/m3d_boundary.h"       // m3dMatrIdentity

// ?MDL_PROP_ST_KEEP_BHV_SMART@@3UctrlPROP_ID@@B — the "keep smart behaviour across teleport" body
// property id, indexed into the body's prop bit-cache. boundary.
extern const ctrlPROP_ID MDL_PROP_ST_KEEP_BHV_SMART;

void aiBRAIN::implTransport(const dsTSTRING<char> &nameTrk, bool /*isUseOrient*/)
{
    dsTSTRING<char> callName("Transport()");
    if (this->IsSSLCommonBackOff(callName, dsFLAGS<SSL_COMMON_BACKOFF, int>{0}))
        return;

    m3dMATR mat = m3dMatrIdentity;

    navWP *wp = aiPlanner->GetNavManager()->FindWP(nameTrk.pBuffer->str);
    if (wp)
    {
        wp->wpData->GetMatrix(&mat);
    }
    else
    {
        entTRACKER trk;
        if (!trk.Init(nameTrk))
        {
            _apLog("~AI,Ssl,Error~'%s' can't call Transport(\"%s\"): can't find wp/object",
                   this->names.nameInst, nameTrk.pBuffer->str);
            return;
        }
        trk.GetMatrix(&mat, 0);
    }

    // Build the pose block: pos = translation row (v[3]), forward = v[2], up = v[1].
    fsmCB_POS_DIR_PAR par;
    par.pos.x = mat.v[3].x;  par.pos.y = mat.v[3].y;  par.pos.z = mat.v[3].z;
    par.dirZ.x = mat.v[2].x; par.dirZ.y = mat.v[2].y; par.dirZ.z = mat.v[2].z;
    par.dirY.x = mat.v[1].x; par.dirY.y = mat.v[1].y; par.dirY.z = mat.v[1].z;
    this->Transport(par);

    this->st.pointee->body.val |= 0x4000000u;
    aiPlanner->LocalizeToNavSystem(this);

    aiBEST_BASE *best = this->mind.best.pointee;
    best->FinishBehavs(AI_BID_MAJOR_PATH);
    best->FinishBehavs(AI_BID_GVT_MAJOR_PATH);
    best->FinishBehavs(AI_BID_PATH);
    best->FinishBehavs(AI_BID_HO);
    best->FinishBehavs(AI_BID_HO_PATH);

    const ctrlPROP_LIST *props = this->body.pointee->GetRefPropCache();
    if (!dsBIT_ARRAY_IMPL::Is(MDL_PROP_ST_KEEP_BHV_SMART.id, props->props.array, 12))
    {
        best->FinishBehavs(AI_BID_SMART);
        best->FinishBehavs(AI_BID_MAJOR_SMART);
        best->FinishBehavs(AI_BID_TURRET);
    }
    best->FinishBehavs(AI_BID_ZERO_PATH);
    best->FinishBehavs(AI_BID_ZERO_HO);
    best->FinishBehavs(AI_BID_TEACH_HO);
    best->ClearB2BParcels();

    _apLog("~AI~'%s' called Transport(\"%s\")", this->names.nameInst, nameTrk.pBuffer->str);
}
