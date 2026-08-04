// ds::LIST<msgADDR *>::DATA::DATA(msgADDR *const &) — 0x82AC5B88
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::DATA::DATA(msgADDR *const &value) {
    val  = value;
    next = nullptr;
    prev = nullptr;
}

} // namespace ds
