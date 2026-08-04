// ds::LIST<msgADDR *>::operator+=(msgADDR *const &) — 0x82AC5F10
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *> &LIST<msgADDR *>::operator+=(msgADDR *const &val) {
    PushBack(val);
    return *this;
}

} // namespace ds
