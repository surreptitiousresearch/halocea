#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::operator= @ 0x827AFE78
// Copy-assign every element from `src`. Self-assign (same backing storage) is a no-op. Otherwise:
// destroy this's current live elements, conditionally Realloc, then Insert()-copy the source
// range. CAVEAT: matching the disasm exactly, the Realloc size argument passed is
// `this->nElem` (which is 0 immediately after the preceding Clear()), not `src->nElem` — i.e. the
// compiled code reallocates down to empty (when allocated != src.nElem) rather than to the source
// size; Insert()'s own doubling-growth logic then re-grows storage as it copies elements in. This
// looks like a source-level quirk (or a `nElem`/`src->nElem` mixup in the original code) rather
// than a decompiler misread — the disasm unambiguously loads `this`'s (post-Clear, zeroed) nElem
// field for the Realloc call. Reproduced verbatim; DB is ground truth.
template<>
dsVECTOR<dsDATA, 8> &dsVECTOR<dsDATA, 8>::operator=(const dsVECTOR<dsDATA, 8> &src)
{
    if (src.pData == this->pData)
        return *this;

    this->Clear();
    if (this->allocated != src.nElem)
        this->Realloc(this->nElem);
    this->Insert(0, src.pData, src.nElem);
    return *this;
}
