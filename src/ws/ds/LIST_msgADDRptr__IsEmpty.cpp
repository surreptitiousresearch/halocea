// ds::LIST<msgADDR *>::IsEmpty() const — 0x82AC58B0
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
bool LIST<msgADDR *>::IsEmpty() const {
    return head == nullptr;
}

} // namespace ds
