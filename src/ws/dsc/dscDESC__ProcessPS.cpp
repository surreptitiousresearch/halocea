#include "dscDESC.h"

// dscDESC::ProcessPS(psSECTION, const dsTSTRING<char>&) @ 0x82663198
// Run this descriptor's virtual two-phase parse over a config-file section: ParsePS, then
// PostProcessPS. `psRead` is a by-value parameter throughout (see the dscDESC_vtbl note in
// dscDESC.h); each virtual call implicitly copy-constructs its own private temporary from it,
// matching the two separate psSECTION copy-constructions seen at 0x826631B8/0x826631E0 in the
// disassembly, and `psRead` itself is destroyed on this function's return (0x82663200).
void dscDESC::ProcessPS(psSECTION psRead, const dsTSTRING<char> &hintErr)
{
    this->__vftable->ParsePS(this, psRead, hintErr);
    this->__vftable->PostProcessPS(this, psRead, hintErr);
}
