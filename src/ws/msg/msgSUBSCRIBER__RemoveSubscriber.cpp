#include "msgSUBSCRIBER.h"
#include "msgADDR.h"
#include "saLIST.h"

// msgSUBSCRIBER::RemoveSubscriber @ 0x82AC6928
// Unsubscribe `pAddr` from every message id: walk the (msg -> subscriber-list) table, and in each
// entry find the cell holding `pAddr` and unlink it; if an entry's list becomes empty, drop the
// whole entry (without advancing, since the vector shifted down).
//
// The decompiler rendered the ds::LIST iterators across overlapping BYREF stack slots; they are
// untangled here into named iterators, following the sibling UnRegisterPair reconstruction.
void msgSUBSCRIBER::RemoveSubscriber(msgADDR *pAddr)
{
    int nEntries = this->msgPairList.nElem;
    int idx = 0;
    while (idx < nEntries) {
        saLIST &pair = this->msgPairList[idx];

        ds::LIST<msgADDR *>::ITERATOR iterStart;
        iterStart.data = pair.addList.head;
        ds::LIST<msgADDR *>::ITERATOR found =
            ds::FindNonSorted<ds::LIST<msgADDR *>::ITERATOR, msgADDR *>(iterStart, pAddr);

        if (found.data) {
            pair.addList.Erase(found);
            if (!this->msgPairList[idx].addList.head) {
                this->msgPairList.Erase(idx, 1);
                --nEntries;
                continue;
            }
        }
        ++idx;
    }
}
