#include "msgSUBSCRIBER.h"
#include "msgSYSTEM.h"
#include "msgCMP_INDEX.h"

// msgSUBSCRIBER::SendMsg @ 0x82AC6718
// Look up the subscriber list for `msg` and, if present, deliver the message to each subscribed
// address through the global message system.
void msgSUBSCRIBER::SendMsg(int msg, void *pInfo, msgADDR *pSender)
{
    msgCMP_INDEX cmp;
    int idx = this->msgPairList.FindSorted<msgCMP_INDEX, int>(msg, cmp);
    if (idx != -1)
    {
        saLIST &pair = this->msgPairList[idx];
        for (ds::LIST<msgADDR *>::DATA *node = pair.addList.head; node; node = node->next)
            msgSystem.SendMsg(node->val, msg, pInfo, pSender);
    }
}
