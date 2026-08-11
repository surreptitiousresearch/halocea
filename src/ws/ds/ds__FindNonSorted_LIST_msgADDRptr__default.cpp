// ds::FindNonSorted<ds::LIST<msgADDR *>::ITERATOR, msgADDR *> — @0x82AC6018
// Default-comparator overload (2 template args): forwards to the explicit-CMP form using the
// default ds::CMP.
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::ITERATOR FindNonSorted<LIST<msgADDR *>::ITERATOR, msgADDR *>(
    const LIST<msgADDR *>::ITERATOR &start,
    msgADDR *const &key)
{
    return FindNonSorted<CMP, LIST<msgADDR *>::ITERATOR, msgADDR *>(start, key);
}

} // namespace ds
