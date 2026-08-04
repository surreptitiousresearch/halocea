#include "dsVECTOR.h"
#include "dsDATA.h"

// dsVECTOR<dsDATA,8>::dsVECTOR(const dsVECTOR&) @ 0x828934C8
// Copy-construct: start empty, adopting src's __cl call-site cookie verbatim (disasm: a raw
// 8-byte copy of src->__cl, NOT "file replaced with this" — the decompiler's rendering of that
// step was a mis-cast), then deep-copy every element from src via Insert (which itself
// StoreValue-copy-constructs each dsDATA rather than memcpy'ing it).
template<>
dsVECTOR<dsDATA, 8>::dsVECTOR(const dsVECTOR<dsDATA, 8> &src)
    : pData(nullptr), nElem(0), allocated(0), __cl(src.__cl)
{
    this->Insert(0, src.pData, src.nElem);
}
