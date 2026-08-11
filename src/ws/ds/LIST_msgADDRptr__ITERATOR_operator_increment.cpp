// ds::LIST<msgADDR *>::ITERATOR::operator++() — @0x82AC5920
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
void LIST<msgADDR *>::ITERATOR::operator++() {
    if (data)
        data = data->next;
}

} // namespace ds
