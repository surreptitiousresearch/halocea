#include "msgSYSTEM.h"
#include "msgADDR.h"
#include "msgCMP_ADDR1.h"
#include "msgCMP_ADDR2.h"
#include <string.h>

// msgSYSTEM::InsertAddr @ 0x8255D680
// Link a freshly-registered address into the message system's priority-bucket vector. The vector
// holds one intrusive ds::PTR_LIST<msgADDR> per priority band, kept sorted by band. `pAddr` starts
// with the lowest possible priority key (0x7FFFFFFF); if a bucket for that key already exists we
// append into it, otherwise we build a new one-element bucket and insert it in sorted order. The
// address is then flagged priority-managed (0x20) and given its uid if it does not have one.
//
// CAVEAT: the decompiler smeared the ds::PTR_LIST::Insert plumbing across several overlapping BYREF
// stack slots; the insert positions (the bucket's tail hook) and the new-bucket construction were
// untangled against the disassembly (0x8255D6CC-0x8255D79C). The list Insert/InsertSorted bodies
// are boundaries.
void msgSYSTEM::InsertAddr(msgADDR *pAddr)
{
    pAddr->priorityInt = 0x7FFFFFFF;

    msgCMP_ADDR1 cmpByPriority;
    int bucketIdx = this->addrList1.FindSorted<msgCMP_ADDR1, msgADDR *>(pAddr, cmpByPriority);

    if (bucketIdx == -1) {
        // No bucket for this priority yet: make a one-element list and insert it sorted.
        ds::PTR_LIST<msgADDR> newBucket;
        memset(&newBucket, 0, sizeof(newBucket));

        ds::PTR_LIST<msgADDR>::ITERATOR tailPos;
        tailPos.pNode = newBucket.pTail;
        newBucket.Insert(pAddr, tailPos);

        msgCMP_ADDR2 cmpByBucket;
        this->addrList1.InsertSorted<msgCMP_ADDR2>(
            newBucket, cmpByBucket, dsVECTOR<ds::PTR_LIST<msgADDR>, 8>::INS_DUP_ERROR);
    } else {
        ds::PTR_LIST<msgADDR> &bucket = this->addrList1[bucketIdx];
        ds::PTR_LIST<msgADDR>::ITERATOR tailPos;
        tailPos.pNode = bucket.pTail;
        bucket.Insert(pAddr, tailPos);
    }

    pAddr->stateAddr.state |= 0x20u;
    if ((pAddr->stateAddr.state & 0x100) == 0)
        this->AssignUID(pAddr);
}
