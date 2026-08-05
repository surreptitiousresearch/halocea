#include <stdint.h>
#include "../../headers/ws/rend/rendDRIVER.h"      // REND_PRIMTYPE + extern "C" DynGeomDraw decl
#include "../../headers/vidDRIVER_DYNGEOM_BUFFER.h" // buffer + vidDriver->drvInterface->dynGeomBuffer chain
#include "../../headers/ws/vid/vidFVF_DESCR.h"      // full layout — sizeOfFVF@0x92 (the fwd decl above is opaque)
#include "../../headers/ws/ds/ds_assert_boundary.h"

// DynGeomDraw @ 0x827C9028  (D:\Projects\code\common\src.sys\render\rend_vis.cpp)
// One-shot immediate-mode draw through the driver's dynamic-geometry buffer. Fits an FVF
// descriptor for `fvf`, opens a fresh chunk, streams the vertices (and, when present, the
// indices) into it, issues the draw, and closes the chunk. On a write failure the chunk is still
// ended (no draw). This is the concrete body behind the 28 call sites; the two rendDRIVER draw
// thunks (DrawIndexedPrimitive / DrawPoly / RenderPoly) all funnel through here.
//
// ABI: the decompiler renders the seven logical scalar/pointer args (primType, vertList, nVert,
// indList, nInd, fvf, callerID) as fewer 64-bit register-pair-aliased parameters. The disasm
// (r3..r9 = primType, vertList, nVert, indList, nInd, fvf, callerID) is authoritative and matches
// the declaration in rendDRIVER.h.

// vid boundary — declared locally to avoid pulling vid_boundary.h (whose `typedef int
// REND_PRIMTYPE` would clash with rendDRIVER.h's enum REND_PRIMTYPE).
struct vidVBUF_MNG;   // vid boundary (opaque here)
struct vidFVF_DESCR;  // vid boundary (opaque here)
extern vidDRIVER *vidDriver;
extern vidVBUF_MNG *vidVBMng;
extern vidFVF_DESCR *vidVBUF_MNG_FitFVF(vidVBUF_MNG *self, unsigned long long fvf);

void DynGeomDraw(REND_PRIMTYPE primType, void *vertList, int nVert,
                 unsigned short *indList, int nInd, uint64_t fvf,
                 unsigned int callerID)
{
    vidDRIVER_DYNGEOM_BUFFER *buffer = vidDriver->drvInterface->dynGeomBuffer;
    if (!IGNORE_STRONG_ASSERT && !buffer)
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
            "buffer",
            "D:\\Projects\\code\\common\\src.sys\\render\\rend_vis.cpp", 34,
            empty_string); // instance-call, 4 args (dropped stray leading this)

    vidFVF_DESCR *descr = vidVBUF_MNG_FitFVF(vidVBMng, fvf);
    if (!IGNORE_STRONG_ASSERT && !descr)
        ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
            "descr",
            "D:\\Projects\\code\\common\\src.sys\\render\\rend_vis.cpp", 37,
            empty_string); // instance-call, 4 args (dropped stray leading this)

    unsigned char sizeOfFVF = descr->sizeOfFVF;   /* 0x92 */

    // Begin() takes the caller's FourCC debug/allocation tag (the decompiler mislabels r25 as
    // HIDWORD(fvf); the disasm shows it is callerID, arg r9).
    vidDRIVER_DYNGEOM_CHUNK *chunk = buffer->__vftable->Begin(buffer, callerID);

    unsigned int vertBase = (unsigned int)-1;
    if (nVert > 0 &&
        !buffer->__vftable->WriteVertices(buffer, chunk, vertList, sizeOfFVF, nVert, &vertBase)) {
        buffer->__vftable->End(buffer, chunk);
        return;
    }

    unsigned int indBase = (unsigned int)-1;
    if (nInd > 0 &&
        !buffer->__vftable->WriteIndices(buffer, chunk, indList, nInd, &indBase)) {
        buffer->__vftable->End(buffer, chunk);
        return;
    }

    buffer->__vftable->Draw(buffer, descr, primType, vertBase, nVert, indBase, nInd);
    buffer->__vftable->End(buffer, chunk);
}
