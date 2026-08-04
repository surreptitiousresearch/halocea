#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiBEST.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsSHARED_PTR.h"
#include "../../headers/ws/ds/ds_new.h"

// aiBEST_BASE::ConvertToFull @ 0x83297B88
// ?ConvertToFull@aiBEST_BASE@@UAAXPAVaiBRAIN@@@Z  (virtual)
//
// Promote the brain's best-behaviour module from the bare/singleton base to a full aiBEST: build a
// new aiBEST for the brain, swap it into pBrain->mind.best (a shared pointer), and release whatever
// module was there before. The swap-then-Reset is the original copy-and-swap the shared_ptr uses,
// preserved verbatim so no spurious reference bump occurs on the freshly-built module.
void aiBEST_BASE::ConvertToFull(aiBRAIN *pBrain)
{
    aiBEST *full = new ("D:\\Projects\\code\\common\\src.sys\\gm_shared\\ai08\\b8s_frame.cpp", 0x36) aiBEST(pBrain);

    dsSHARED_PTR<aiBEST_BASE, 0, Deleter<aiBEST_BASE> > sp(full);

    aiBEST_BASE *tmpPointee = sp.pointee;
    int          tmpCounter = sp.counter;
    sp.pointee = pBrain->mind.best.pointee;
    sp.counter = pBrain->mind.best.counter;
    pBrain->mind.best.pointee = tmpPointee;
    pBrain->mind.best.counter = tmpCounter;

    sp.Reset(); // release the previous module (now held by sp)
}
