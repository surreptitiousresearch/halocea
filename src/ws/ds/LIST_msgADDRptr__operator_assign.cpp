// ds::LIST<msgADDR *>::operator=(const LIST<msgADDR *> &) — @0x82AC61E8
// Mangle (QAAXABV01@) marks the return type void, not the usual LIST&, so the reconstruction
// keeps a void return to match. Disasm: after Clear(), the compiler placement-reconstructs `this`
// via the copy ctor (the decompiler's `if (this)` around that call is a residual, always-true
// null-check on `this` and carries no real semantics here).
#include "LIST.h"
#include "../msg/msgADDR.h"
#include <new> // placement new

namespace ds {

template<>
void LIST<msgADDR *>::operator=(const LIST<msgADDR *> &other) {
    if (this != &other) {
        Clear();
        new (this) LIST<msgADDR *>(other);
    }
}

} // namespace ds
