// ds::LIST<msgADDR *>::ITERATOR::operator*() const — 0x82AC5950
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::DATA *LIST<msgADDR *>::ITERATOR::operator*() const {
    return data;
}

} // namespace ds
