#include "dsVECTOR.h"

class objOBJ; // boundary — obj subsystem game object (element is a pointer)

// dsVECTOR<objOBJ*,8>::operator= @ 0x826890B8
// Self-assignment no-op; otherwise resets nElem to 0 (deferring to Realloc/Insert to
// rebuild storage), reallocs only if current allocated capacity differs from src's
// element count, then bulk-inserts src's element pointers at position 0.
// CAVEAT: disassembly shows a counting loop (r11 from 0 to this->nElem) with an empty
// body before the reset — a vestigial no-op iteration (pointer elements have a trivial
// destructor, so the per-element cleanup the loop shape implies compiles away to
// nothing); reproduced faithfully below rather than silently dropped.
template<>
dsVECTOR<objOBJ *, 8> &dsVECTOR<objOBJ *, 8>::operator=(const dsVECTOR<objOBJ *, 8> &src)
{
    if (src.pData != this->pData) {
        int priorElemCount = this->nElem;
        for (int i = 0; i < priorElemCount; ++i)
            ; // no-op: trivial destructor for pointer elements

        int priorAllocated = this->allocated;
        this->nElem = 0;
        if (priorAllocated != src.nElem)
            this->Realloc(0);
        this->Insert(0, src.pData, src.nElem);
    }
    return *this;
}
