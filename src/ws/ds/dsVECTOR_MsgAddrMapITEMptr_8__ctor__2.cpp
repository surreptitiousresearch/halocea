#include "dsVECTOR.h"

#include "dsMsgAddrMapITEM.h"

// dsVECTOR<...msgADDR-map ITEM*,8>::dsVECTOR(int, ITEM *const &, apCL) @ 0x8255BB90
// Construct with an initial live size: zero-init the fields, adopt `cl` (copied by value into
// __cl — DEVIATION corrected same as the 1-arg ctor sibling, confirmed by the disassembly's `std
// r6, arg_28(r1)` 8-byte store of the by-value apCL followed by a plain field-pair reload/store
// into this+0xC/this+0x10), then delegate to Resize(size, fill) to grow/fill to `size` elements.
// NOTE: this ctor overload (int size, const T &fill, apCL cl) is not yet declared in dsVECTOR.h's
// generic ctor list — the header only documents dsVECTOR(int size, const apCL &cl); this is a
// distinct fill-construct overload confirmed by the mangle (H ABQAUITEM... UapCL@@).
template<>
dsVECTOR<dsMsgAddrMapITEM *, 8>::dsVECTOR(int size, dsMsgAddrMapITEM *const &fill, const apCL &cl)
{
    this->pData = 0;
    this->nElem = 0;
    this->allocated = 0;
    this->__cl.file = cl.file;
    this->__cl.line = cl.line;
    this->Resize(size, fill);
}
