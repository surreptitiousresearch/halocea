#include "msgSYSTEM.h"
#include "../ds/dsCMP.h"
#include "../ds/ds_assert_boundary.h"

// msgSYSTEM::RemoveFromQueues @ 0x82559EF0
// Purge every reference to `pAddr`: drop it from the change-priority list (if queued) and from
// both the per-frame and time-delayed message queues.
void msgSYSTEM::RemoveFromQueues(msgADDR *pAddr)
{
    if (!IGNORE_STRONG_ASSERT && !pAddr)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "pAddr",
            "D:\\Projects\\code\\common\\src.sys\\gm_sys\\msg\\msgaddr.cpp",
            627,
            empty_string);

    dsCMP    cmp;
    msgADDR *key = pAddr;

    int idx = msgSystem.listChangePrior.FindSorted<dsCMP, msgADDR *>(key, cmp);
    if (idx != -1)
        this->listChangePrior.Erase(idx, 1);

    this->msgQueue.DeleteMsgByAddr(pAddr);
    this->timeQueue.DeleteMsgByAddr(pAddr);
}
