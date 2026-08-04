#include "../../headers/ws/txm/txmMANAGER.h"
#include "../../headers/ws/ds/dsSWAP.h"

// 0x826FAFD8 -- ?Find@txmMANAGER@@QAAPAVtxmTEXTURE@@PBD@Z
// Name-keyed lookup in the manager's texture list. When entries have been appended out of order
// (listNeedSort), the list is first re-sorted (txmCMP_TEX_SORT) and any trailing null slots left
// by prior removals are trimmed off; then a binary search by name (txmCMP_TEX_FIND) locates the
// texture. Returns nullptr when the name isn't present.
//
// The decompiler's spurious a3..a14 tail parameters are register-spill artifacts of the inlined
// dsVECTOR templates; the real ABI is (this, name). `nameKey` mirrors the decompiler's `a14`
// stack copy of `name` -- FindSorted is handed its address (a const char ** key).
txmTEXTURE *txmMANAGER::Find(const char *name)
{
    const char *nameKey = name;

    if (this->listNeedSort)
    {
        txmCMP_TEX_SORT sortCmp;
        dsSWAP<txmTEXTURE *> swap;
        this->listTex.Sort(sortCmp, swap);

        // Trailing null slots sort to the end; find the last live element and trim.
        int i;
        for (i = this->listTex.nElem - 1; i >= 0; --i)
        {
            if (this->listTex[i])
                break;
        }
        this->listTex.Resize(i + 1);
        this->listNeedSort = 0;
    }

    txmCMP_TEX_FIND findCmp;
    int idx = this->listTex.FindSorted(nameKey, findCmp);
    if (idx == -1)
        return nullptr;
    return this->listTex[idx];
}
