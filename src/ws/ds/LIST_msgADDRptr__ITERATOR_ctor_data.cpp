// ds::LIST<msgADDR *>::ITERATOR::ITERATOR(DATA *) — private, cell-wrapping ctor — 0x82AC5960
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::ITERATOR::ITERATOR(LIST<msgADDR *>::DATA *fromData) {
    data = fromData;
}

} // namespace ds
