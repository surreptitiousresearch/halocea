// ds::LIST<msgADDR *>::ITERATOR::IsDone() const — @0x82AC5900
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
bool LIST<msgADDR *>::ITERATOR::IsDone() const {
    return data == nullptr;
}

} // namespace ds
