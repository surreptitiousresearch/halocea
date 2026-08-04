// ds::LIST<msgADDR *>::`scalar deleting destructor' — 0x82AC5E28
// Compiler-generated (MSVC ??_G) thunk: not user-representable in the LIST<T> template, so it is
// reconstructed here as the free-standing thunk it actually is rather than a declared member.
// bit 0 of `flags` selects "delete the object too" vs "destroy in place only".
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

void *LIST_msgADDRptr_ScalarDeletingDestructor(LIST<msgADDR *> *self, unsigned int flags) {
    self->~LIST<msgADDR *>();
    if (flags & 1)
        operator delete(self);
    return self;
}

} // namespace ds
