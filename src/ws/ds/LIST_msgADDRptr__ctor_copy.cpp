// ds::LIST<msgADDR *>::LIST(const LIST<msgADDR *> &) — @0x82AC6058
// Deviation: decompiler mangled the __cl copy into
// `cl.file = (const char *)this; *(apCL *)(cl.file + 12) = cl;`. Disasm shows a plain 64-bit
// `ld`/`std` of other->__cl into this->__cl (whole-struct copy), same as the apCL(cl) ctor.
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::LIST(const LIST<msgADDR *> &other) {
    length = 0;
    head   = nullptr;
    tail   = nullptr;
    __cl   = other.__cl;
    for (DATA *node = other.head; node; node = node->next)
        PushBack(node->val);
}

} // namespace ds
