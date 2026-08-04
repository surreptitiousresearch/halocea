#include "dsVECTOR.h"
#include "dsDATA.h"
#include "dsSWAP.h"
#include "dsREF_VECTOR.h"

// dsVECTOR<dsDATA,8>::Sort<dsREF_VECTOR::SortCmp,dsSWAP<dsDATA>> @ 0x8261BF80
// In-place sort ordered by `cmp` (dsREF_VECTOR::SortCmp::cmp, a three-way comparator). Unlike the
// lighter instantiations (e.g. psSECTION_RECORD's Sort, which only permutes an index field), a
// dsDATA element carries its own runtime-typed value, so `swap`'s exchange is realised here as a
// full 3-way StoreValue dance through a stack scratch (construct scratch=a, a=b, b=scratch,
// destroy scratch) rather than a raw word swap — matching the disasm exactly. For up to 100
// elements this uses an O(n^2) selection sort; larger vectors delegate to HeapSort. `nElem` is
// reloaded every inner iteration, matching the disasm (cmp/swap could in principle resize this).
template<>
template<>
void dsVECTOR<dsDATA, 8>::Sort<dsREF_VECTOR::SortCmp, dsSWAP<dsDATA>>(
        const dsREF_VECTOR::SortCmp &cmp, const dsSWAP<dsDATA> &swap)
{
    int nElem = this->nElem;
    if (nElem <= 100) {
        for (int i = 0; i < nElem - 1; ++i) {
            for (int j = i + 1; j < nElem; ++j) {
                if (cmp.cmp(this->pData[j], this->pData[i]) < 0) {
                    // Full-value 3-way swap (dsDATA is not trivially exchanged): stage pData[i]
                    // into a scratch, move pData[j] into pData[i], move the scratch into pData[j].
                    dsDATA scratch;
                    scratch.type = nullptr;
                    scratch.StoreValue(this->pData[i]);
                    this->pData[i].StoreValue(this->pData[j]);
                    this->pData[j].StoreValue(scratch);
                    if (scratch.type)
                        scratch.type->Destroy((unsigned int *)&scratch);
                }
                nElem = this->nElem;
            }
        }
    } else {
        this->HeapSort(cmp, swap);
    }
}
