#include "../../headers/ws/rend/rendDRIVER.h"

// rendDRIVER::DrawIndexedPrimitive @ 0x827C91D8
// Thin thunk: forwards to DynGeomDraw as a triangle list. The disasm computes the index
// count as 3*nFace (0x827C91D8: slwi r11,r7,1 ; add r7,r7,r11) and tail-branches into
// DynGeomDraw. `unusedArg` and the driver `this` are not touched by the thunk itself.
void rendDRIVER::DrawIndexedPrimitive(void *vertList, int nVert, unsigned short *indList,
                                      int nFace, int unusedArg, unsigned __int64 fvf,
                                      unsigned int callerID)
{
    DynGeomDraw(REND_PRIMTYPE_TRILIST, vertList, nVert, indList, 3 * nFace, fvf, callerID);
}
