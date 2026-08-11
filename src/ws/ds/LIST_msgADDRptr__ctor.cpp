// ds::LIST<msgADDR *>::LIST(apCL) — @0x82AC5980
// Deviation: decompiler mis-cast the apCL copy as
// `__cl.file = (const char *)this; __cl.line = (int)cl;`. Disasm shows `cl` (apCL, 8 bytes:
// {file@0,line@4}) arrives packed whole in r4 and is copied verbatim to this->__cl (a single
// 64-bit `std`/`ld` pair over the two 32-bit fields) — i.e. a plain struct copy, not a
// this-pointer stash.
#include "LIST.h"
#include "../msg/msgADDR.h"

namespace ds {

template<>
LIST<msgADDR *>::LIST(apCL cl) {
    length = 0;
    head   = nullptr;
    tail   = nullptr;
    __cl   = cl;
}

} // namespace ds
