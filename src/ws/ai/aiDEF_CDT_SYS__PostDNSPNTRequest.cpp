#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiCDT_REQ_PARAMS.h"
#include "../../headers/ws/ai/aidr_requests_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../../headers/ws/ds/ds_new.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared empty-string / assert info byte @0x8200155A

// aiDEF_CDT_SYS::PostDNSPNTRequest @ 0x8323F058
// ?PostDNSPNTRequest@aiDEF_CDT_SYS@@UAAXABUaiDNS_PNT_REQ_PARAMS@@@Z
//
// Enqueue a "does-not-see-point" request (DRDNS_PNT) keyed by (sender, DR_DNS_PNT, -1).
void aiDEF_CDT_SYS::PostDNSPNTRequest(const aiDNS_PNT_REQ_PARAMS &params)
{
    aiBRAIN *sender = params.pSender;
    float ttl = sender->st.pointee->lod.timeKeepRequests;

    aidr::DRDNS_PNT *req = new (
        "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_def_cdt_sys.cpp", 0x1EB)
        aidr::DRDNS_PNT(ttl);
    aidr::DR *dr = req;
    if (!IGNORE_STRONG_ASSERT && !dr)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "dr",
            "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_def_cdt_sys.cpp", 492,
            byte_8200155A);
    req->PostParams(params); // secondary request-param interface virtual (vtable[1] slot 0)

    keyREQ key;
    key.pSender = sender;
    key.pSenderDbg.SetPtr(sender);
    key.typeReq = aidr::DR_DNS_PNT;
    key.sgidx = -1;
    srcReqs.InsertNew(&key, &dr);
}
