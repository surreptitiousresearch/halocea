// ds::FindNonSorted<ds::CMP, ds::LIST<msgADDR *>::ITERATOR, msgADDR *> — 0x82AC5E70
// Explicit-comparator overload: the decompile shows a direct `back_chain->val == *val` compare,
// not a call through ds::CMP — the CMP::equals<msgADDR*,msgADDR*> functor call was inlined by the
// compiler into the raw pointer comparison. Reconstructed as the same direct comparison to match
// the disassembly (a CAVEAT: semantically this stands in for CMP::equals(a,b), but no call site
// remains to reverse separately).
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::ITERATOR FindNonSorted<CMP, LIST<msgADDR *>::ITERATOR, msgADDR *>(
    const LIST<msgADDR *>::ITERATOR &start,
    msgADDR *const &key)
{
    for (LIST<msgADDR *>::DATA *cell = *start; cell; cell = cell->next) {
        if (cell->val == key) {
            LIST<msgADDR *>::ITERATOR found;
            found.data = cell;
            return found;
        }
    }
    LIST<msgADDR *>::ITERATOR notFound;
    notFound.data = nullptr;
    return notFound;
}

} // namespace ds
