// ds::LIST<msgADDR *>::~LIST() — @0x82557B40
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::~LIST() {
    Clear();
}

} // namespace ds
