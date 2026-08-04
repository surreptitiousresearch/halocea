#include <string.h>
#include <new>
#include "dsVECTOR.h"
#include "../msg/saLIST.h"
#include "ds_assert_boundary.h"

// dsVECTOR<saLIST,8>::Insert(pos,val,n) @ 0x82AC6B80
// Fill-insert: splice `n` copies of the single record `val` in at index `pos`. When there is
// already spare capacity, shift the tail up (memmove) and construct each new slot directly from
// `val` (msg copied by value, addList deep-copied via its copy ctor). Otherwise deep-copy `val`
// into a local temporary FIRST (in case `val` aliases storage about to be reallocated), grow,
// shift, construct each new slot from the temporary, then release the temporary's addList.
template<>
void dsVECTOR<saLIST, 8>::Insert(int pos, const saLIST &val, int n)
{
    if (!IGNORE_STRONG_ASSERT && (pos < 0 || pos > this->nElem || n < 0))
        STRONG_ASSERT2_HELPER::asserd<int, int, int>(
            "pos >=0 && pos <= nElem && n >= 0",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp",
            338,
            "pos", pos, "nElem", this->nElem, "n", n);
    if (n == 0)
        return;

    if (this->allocated >= this->nElem + n) {
        memmove(&this->pData[pos + n], &this->pData[pos], sizeof(saLIST) * (this->nElem - pos));
        for (int i = pos; i < pos + n; ++i) {
            saLIST *slot = &this->pData[i];
            slot->msg = val.msg;
            new (&slot->addList) ds::LIST<msgADDR *>(val.addList);
        }
        this->nElem += n;
        return;
    }

    int msg = val.msg;
    ds::LIST<msgADDR *> addListCopy(val.addList); // deep-copy before Realloc may invalidate `val`
    int allocated = this->allocated;
    int needed = this->nElem + n;
    if (2 * allocated > needed)
        needed = 2 * allocated;
    if (needed > allocated)
        this->Realloc(needed);

    memmove(&this->pData[pos + n], &this->pData[pos], sizeof(saLIST) * (this->nElem - pos));
    for (int i = pos; i < pos + n; ++i) {
        saLIST *slot = &this->pData[i];
        slot->msg = msg;
        new (&slot->addList) ds::LIST<msgADDR *>(addListCopy);
    }
    addListCopy.Clear();

    this->nElem += n;
}
