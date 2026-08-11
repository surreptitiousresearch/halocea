// ds::LIST<msgADDR *>::Begin() const — @0x82AC5BC0 (lowest address of the two Begin overloads)
// Deviation: the raw decompile swapped the sret-return-slot register (r3, labeled "this") and the
// real `this` (r4, labeled "result") — verified against disasm: r3 is the hidden ITERATOR return
// slot, r4 is `this`. Untangled to the real logic: result.data = this->head.
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::ITERATOR LIST<msgADDR *>::Begin() const {
    ITERATOR result;
    result.data = head;
    return result;
}

} // namespace ds
