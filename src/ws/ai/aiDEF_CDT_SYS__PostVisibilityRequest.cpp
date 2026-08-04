#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiCDT_REQ_PARAMS.h"
#include "../../headers/ws/ai/aidr_requests_boundary.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/dbg/STRONG_ASSERT_DUMMY.h"
#include "../../headers/ws/ds/ds_new.h"

extern int IGNORE_STRONG_ASSERT;    // _IGNORE_STRONG_ASSERT
extern unsigned char byte_8200155A; // shared empty-string / assert info byte @0x8200155A

#define A8_DEF_CDT_SYS_CPP "D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\a8_def_cdt_sys.cpp"

// aiDEF_CDT_SYS::PostVisibilityRequest @ 0x8323EB20
// ?PostVisibilityRequest@aiDEF_CDT_SYS@@UAAXABUaiVIS_REQ_PARAMS@@@Z
//
// Enqueue a visibility request. The concrete request class is chosen from the sender's vision
// module and the check distance: snipers and far checks use the "long" variant, close/simplest
// checks use the cheap circular variant, mid-range uses the sliced variant.
void aiDEF_CDT_SYS::PostVisibilityRequest(const aiVIS_REQ_PARAMS &params)
{
    aiBRAIN *sender = params.sender;

    float dist = sender->aiBRAIN_IFACE::Dist(&params.posCheck);
    aiVISION_BASE *vision = sender->mind.vision.pointee;
    bool isSniper = vision->IsSniper();
    bool needSimplest = dist < 10.0f || vision->IsNeedSimplestVision();

    // Reset the per-brain debug vision line list (count only; storage retained).
    sender->dbg.vision.lines.nElem = 0;

    float ttl = sender->st.pointee->lod.timeKeepRequests;

    aidr::DR *dr;
    if (isSniper)
    {
        aidr::DRVIS_LONG *req = new (A8_DEF_CDT_SYS_CPP, 325) aidr::DRVIS_LONG(ttl);
        if (req) req->PostParams(params);
        dr = req;
    }
    else if (needSimplest)
    {
        aidr::DRVIS_CIRC *req = new (A8_DEF_CDT_SYS_CPP, 0x149) aidr::DRVIS_CIRC(ttl);
        if (req) req->PostParams(params);
        dr = req;
    }
    else if (dist >= 11.0f)
    {
        aidr::DRVIS_LONG *req = new (A8_DEF_CDT_SYS_CPP, 333) aidr::DRVIS_LONG(ttl);
        if (req) req->PostParams(params);
        dr = req;
    }
    else
    {
        aidr::DRVIS_SLICED *req = new (A8_DEF_CDT_SYS_CPP, 0x14B) aidr::DRVIS_SLICED(ttl);
        if (req) req->PostParams(params);
        dr = req;
    }

    if (!IGNORE_STRONG_ASSERT && !dr)
        STRONG_ASSERT_DUMMY::Crash(nullptr, "dr", A8_DEF_CDT_SYS_CPP, 336, byte_8200155A);

    keyREQ key;
    key.pSender = sender;
    key.pSenderDbg.SetPtr(sender);
    key.typeReq = aidr::DR_VISIBILITY;
    key.sgidx = -1;
    srcReqs.InsertNew(&key, &dr);
}
