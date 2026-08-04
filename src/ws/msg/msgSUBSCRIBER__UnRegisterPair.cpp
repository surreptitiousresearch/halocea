#include "msgSUBSCRIBER.h"
#include "msgCMP_INDEX.h"

// msgSUBSCRIBER::UnRegisterPair @ 0x82AC65F8
// Unsubscribe `pAddr` from `msg`: find the subscriber list keyed on `msg`, locate the cell
// holding `pAddr`, unlink it, and drop the whole (msg -> list) entry if the list becomes empty.
//
// The decompiler rendered the ds::LIST iterators across several overlapping BYREF stack slots;
// they are untangled here into named iterators. `addList.Erase` returns an iterator to the cell
// following the erased one (null when the list is now empty).
void msgSUBSCRIBER::UnRegisterPair(msgADDR *pAddr, int msg)
{
    msgCMP_INDEX cmp;
    int idx = this->msgPairList.FindSorted<msgCMP_INDEX, int>(msg, cmp);
    if (idx != -1)
    {
        saLIST &pair = this->msgPairList[idx];

        ds::LIST<msgADDR *>::ITERATOR iterStart;
        iterStart.data = pair.addList.head;

        ds::LIST<msgADDR *>::ITERATOR found =
            ds::FindNonSorted<ds::LIST<msgADDR *>::ITERATOR, msgADDR *>(iterStart, pAddr);

        ds::LIST<msgADDR *>::ITERATOR remaining = pair.addList.Erase(found);

        if (!remaining.data)
            this->msgPairList.Erase(idx, 1);
    }
}
