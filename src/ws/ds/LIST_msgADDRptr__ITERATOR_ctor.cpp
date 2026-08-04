// ds::LIST<msgADDR *>::ITERATOR::ITERATOR() — default ctor — 0x82AC5BE8
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::ITERATOR::ITERATOR() {
    data = nullptr;
}

} // namespace ds
