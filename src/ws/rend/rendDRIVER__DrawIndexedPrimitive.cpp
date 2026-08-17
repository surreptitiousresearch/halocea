#include <stdint.h>
#include "../../headers/ws/rend/rendDRIVER.h"

// rendDRIVER::DrawIndexedPrimitive @ 0x827C91D8
// Thin thunk: forwards to DynGeomDraw as a triangle list. The disasm computes the index
// count as 3*nFace (0x827C91D8: slwi r11,r7,1 ; add r7,r7,r11) and tail-branches into
// DynGeomDraw. Only the driver `this` is untouched by the thunk.
// DEVIATION: the previously declared `int unusedArg` was a phantom slot — the mangled name
// ?DrawIndexedPrimitive@rendDRIVER@@QAAXPAXHPAGH_KK@Z decodes to exactly six params, fvf is a
// single 64-bit GPR (r8; `ld r8` @0x83322C58 at a call site) and callerID is r9 (FourCC values
// "UI  "/"RAIN" observed); no call site sets r10. Refuted 2026-08-18.
void rendDRIVER::DrawIndexedPrimitive(void *vertList, int nVert, unsigned short *indList,
                                      int nFace, uint64_t fvf, unsigned int callerID)
{
    DynGeomDraw(REND_PRIMTYPE_TRILIST, vertList, nVert, indList, 3 * nFace, fvf, callerID);
}
