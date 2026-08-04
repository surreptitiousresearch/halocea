// ds::LIST<msgADDR *>::Begin() (non-const overload) — 0x82AC5C88
// Identical body to the const overload (see LIST_msgADDRptr__Begin.cpp for the sret/this
// register-swap deviation note); disasm at this address is byte-for-byte the same shape.
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::ITERATOR LIST<msgADDR *>::Begin() {
    ITERATOR result;
    result.data = head;
    return result;
}

} // namespace ds
