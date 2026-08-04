// ds::LIST<msgADDR *>::Erase(ITERATOR) — 0x82AC59D0
// Deviation: the raw decompile swapped the sret-return-slot register (r3) and `this` (r4),
// labeling the sret slot "this" and `this` as "result[1]/result[2]" — an OVERLAPPED-looking
// artifact that is really just misattributed register roles. Verified against disasm: r3 is the
// hidden ITERATOR return slot, r4 is `this`, r5 is `where` (passed by pointer per the `V312@`
// by-value mangle). Untangled below: `this->head`/`this->tail` are the true targets of the
// "result[1].data"/"result[2].data" stores, `this->length` is the true target of the trailing
// decrement, and the sret slot (data = next) is the real return value.
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::ITERATOR LIST<msgADDR *>::Erase(LIST<msgADDR *>::ITERATOR where) {
    DATA *cell = where.data;
    DATA *next = cell->next;

    if (cell->prev)
        cell->prev->next = cell->next;
    else
        head = cell->next;

    if (cell->next)
        cell->next->prev = cell->prev;
    else
        tail = cell->prev;

    operator delete(cell);
    --length;

    ITERATOR result;
    result.data = next;
    return result;
}

} // namespace ds
