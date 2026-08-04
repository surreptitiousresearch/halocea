#include "dsVECTOR.h"
#include "../gs/gsSTRINGS.h"

// dsVECTOR<gsSTRINGS::Record,8>::FindSorted<gsSTRINGS::RecordCmp,const char*> @ 0x826B8C10
// Binary search of mapTextIdToIdx (ascending, ordered by RecordCmp::cmp) for the record
// whose ID text case-insensitively matches `key`; returns its index or -1. const method
// (QBA mangle).
template<>
template<>
int dsVECTOR<gsSTRINGS::Record, 8>::FindSorted<gsSTRINGS::RecordCmp, const char *>(
        const char *const &key, const gsSTRINGS::RecordCmp &cmp) const
{
    int lo = 0;
    int hi = this->nElem;
    while (lo != hi)
    {
        int mid = (lo + hi) >> 1;
        int cmpResult = cmp.cmp(this->pData[mid], key);
        if (cmpResult <= 0)
        {
            if (cmpResult >= 0)
                return mid;   // exact match
            lo = mid + 1;     // key sorts after mid
        }
        else
        {
            hi = mid;         // key sorts before mid
        }
    }
    return -1;
}
