/* ?FindRecThisOnly@psSECTION_DATA@@IAAHVdsSTRID@@@Z @0x8251A768 */
#include "psSECTION_DATA.h"
#include "psSECTION_RECORD.h"
#include "../ds/dsSTRID.h"
#include "../ds/dsVECTOR.h"
// 0x8251A768 — index of the record whose key equals `key` in THIS section only (no parent walk).
// state&4 selects an unsorted reverse linear scan (returning the raw record index); otherwise the
// table is kept sorted by interned key id (lazily re-sorted when state&8 is set) and a binary search
// returns the record's sort-order index. -1 when absent. Protected (IAA).

int psSECTION_DATA::FindRecThisOnly(dsSTRID key)
{
    unsigned char sectionState = this->state;

    if (sectionState & 4) {
        // Unsorted: reverse linear scan by interned key id, returning the raw record index.
        for (int i = this->orderedRecords.nElem - 1; i >= 0; --i) {
            if (key.id == this->orderedRecords[i].keyStrId.id)
                return i;
        }
        return -1;
    }

    if (sectionState & 8) {
        // Sort-dirty: re-sort the record table by key id, then clear the dirty flag.
        ORS_CMP cmp;
        cmp.arr = &this->orderedRecords;
        ORS_SWAP swap;
        this->orderedRecords.Sort<ORS_CMP, ORS_SWAP>(cmp, swap);
        this->state &= ~8u;
    }

    ORS_CMP cmp;
    cmp.arr = &this->orderedRecords;
    int found = this->orderedRecords.FindSorted<ORS_CMP, dsSTRID>(key, cmp);
    if (found != -1)
        return this->orderedRecords[found].sortedIdx;
    return found;
}
