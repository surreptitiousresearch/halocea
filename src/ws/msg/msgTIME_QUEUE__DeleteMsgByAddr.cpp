#include "msgTIME_QUEUE.h"
#include "msgADDR.h"
#include "msgMSG_TIME.h"
#include <stddef.h>

// Recover the msgMSG_TIME that embeds an intrusive list node (listNode @ offset 24).
static msgMSG_TIME *node_to_msg(ds::PTR_LIST_NODE *node)
{
    return node ? (msgMSG_TIME *)((char *)node - offsetof(msgMSG_TIME, listNode)) : nullptr;
}

// msgTIME_QUEUE::DeleteMsgByAddr @ 0x82AC4560
// Walk the pending delayed-message list and cancel every message aimed at `pAddr`: erase it (after
// decrementing the ready count for messages already due, and advancing the dispatch cursor off it
// if it was the cursor), then free it. Messages merely sent by `pAddr` keep their slot but have
// their sender cleared.
void msgTIME_QUEUE::DeleteMsgByAddr(msgADDR *pAddr)
{
    msgMSG_TIME *msg = this->msgList.pHead;
    while (msg) {
        if (msg->pRecv == pAddr) {
            if (msg->elapsedTime <= 0.0f)
                --this->nMsgReady;

            // If this is the message the dispatch cursor points at, step the cursor forward first.
            if (msg == this->itNextMsg.pNode)
                this->itNextMsg.pNode = node_to_msg(msg->listNode.pNext);

            msgMSG_TIME *next = node_to_msg(msg->listNode.pNext);

            ds::PTR_LIST<msgMSG_TIME>::ITERATOR pos;
            pos.pNode = msg;
            this->msgList.Erase(pos); // Erase takes ITERATOR by value

            msgMSG_TIME *toFree = msg;
            this->FreeMsg(&toFree);

            msg = next;
        } else {
            if (msg->pSend == pAddr)
                msg->pSend = nullptr;
            msg = node_to_msg(msg->listNode.pNext);
        }
    }
}
