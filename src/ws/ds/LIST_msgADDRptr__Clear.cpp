// ds::LIST<msgADDR *>::Clear() — @0x82556838
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
void LIST<msgADDR *>::Clear() {
    while (head) {
        DATA *cell = head;
        head = cell->next;
        operator delete(cell);
    }
    tail   = nullptr;
    head   = nullptr;
    length = 0;
}

} // namespace ds
