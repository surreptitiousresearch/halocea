#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiCDT_REQ_PARAMS.h"
#include "../../headers/ws/ai/aidr_requests_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../../headers/ws/ds/ds_new.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared empty-string / assert info byte @0x8200155A

// aiDEF_CDT_SYS::PostReachRequest @ 0x8323E9B8
// ?PostReachRequest@aiDEF_CDT_SYS@@UAAXABUaiREACH_REQ_PARAMS@@@Z
//
// Enqueue a reachability request: build a DRREACH_SIMPLE (from the sender's per-brain
// keep-request TTL and a geometric-motion refine), copy the params into it, and insert it into
// srcReqs keyed by (sender, DR_REACHABILITY, -1).
void aiDEF_CDT_SYS::PostReachRequest(const aiREACH_REQ_PARAMS &params)
{
    aiBRAIN *sender = params.sender;
    float ttl = sender->st.pointee->lod.timeKeepRequests;

    aidr::DRREACH_SIMPLE *req = new (
        "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_def_cdt_sys.cpp", 0x116)
        aidr::DRREACH_SIMPLE(ttl, aiREFINE_GEO_MOTION(params.sender));

    aidr::DR *dr = req;
    if (!IGNORE_STRONG_ASSERT && !dr)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "dr",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_def_cdt_sys.cpp", 279,
            byte_8200155A);
    req->PostParams(params); // secondary request-param interface virtual (vtable[1] slot 0)

    keyREQ key;
    key.pSender = sender;
    key.pSenderDbg.SetPtr(sender);
    key.typeReq = aidr::DR_REACHABILITY;
    key.sgidx = -1;
    srcReqs.InsertNew(&key, &dr);
}
