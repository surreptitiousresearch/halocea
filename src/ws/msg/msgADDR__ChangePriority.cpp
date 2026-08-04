#include "msgADDR.h"
#include "msgSYSTEM.h"
#include "../ds/dsCMP.h"

// msgADDR::ChangePriority @ 0x8255B438
// Queue this address for a priority re-sort: if it is not already present in the message
// system's change-priority list, insert it (keeping the list sorted). A duplicate would be a
// contract violation, hence INS_DUP_ERROR.
void msgADDR::ChangePriority()
{
    dsCMP    cmp;      // stateless comparator (empty functor; decompiler zero-inits it)
    msgADDR *key = this;

    if (msgSystem.listChangePrior.FindSorted<dsCMP, msgADDR *>(key, cmp) == -1)
    {
        dsCMP    insertCmp;
        msgADDR *entry = this;
        msgSystem.listChangePrior.InsertSorted<dsCMP>(
            entry, insertCmp, dsVECTOR<msgADDR *, 8>::INS_DUP_ERROR);
    }
}
