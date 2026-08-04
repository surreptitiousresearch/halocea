#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiCDT_REQ_PARAMS.h"
#include "../../headers/ws/ai/aidr_requests_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../../headers/ws/ds/ds_new.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared empty-string / assert info byte @0x8200155A

#define A8_DEF_CDT_SYS_CPP "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_def_cdt_sys.cpp"

// aiDEF_CDT_SYS::PostTrajectoryRequest @ 0x8323EEA0
// ?PostTrajectoryRequest@aiDEF_CDT_SYS@@UAAXABUaiTRAJ_REQ_PARAMS@@@Z
//
// Enqueue a trajectory request. A multi-point (>3) path uses the parabolic solver; a short path
// uses the linear solver. Keyed by (sender, DR_TRAJECTORY, params.sgidx).
void aiDEF_CDT_SYS::PostTrajectoryRequest(const aiTRAJ_REQ_PARAMS &params)
{
    aiBRAIN *sender = params.pSender;
    float ttl = sender->st.pointee->lod.timeKeepRequests;

    aidr::DR *dr;
    if (params.points.nEntry > 3)
    {
        aidr::DRTRAJ_PARABOLIC *req = new (A8_DEF_CDT_SYS_CPP, 0x196)
            aidr::DRTRAJ_PARABOLIC(ttl, params.refineNear);
        if (req) req->PostParams(params);
        dr = req;
    }
    else
    {
        aidr::DRTRAJ_LINEAR *req = new (A8_DEF_CDT_SYS_CPP, 0x194)
            aidr::DRTRAJ_LINEAR(ttl, params.refineNear, params.refineFar);
        if (req) req->PostParams(params);
        dr = req;
    }

    if (!IGNORE_STRONG_ASSERT && !dr)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "dr", A8_DEF_CDT_SYS_CPP, 408, byte_8200155A);

    keyREQ key;
    key.pSender = sender;
    key.pSenderDbg.SetPtr(sender);
    key.typeReq = aidr::DR_TRAJECTORY;
    key.sgidx = params.sgidx;
    srcReqs.InsertNew(&key, &dr);
}
