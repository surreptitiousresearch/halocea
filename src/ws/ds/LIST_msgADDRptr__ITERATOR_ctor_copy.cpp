// ds::LIST<msgADDR *>::ITERATOR::ITERATOR(const ITERATOR &) — copy ctor — 0x82AC58E0
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::ITERATOR::ITERATOR(const LIST<msgADDR *>::ITERATOR &other) {
    data = other.data;
}

} // namespace ds
