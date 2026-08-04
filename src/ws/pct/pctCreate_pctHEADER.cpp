// ?pctCreate@@YAPAUpctPICTURE@@PAUpctHEADER@@@Z  (0x827C42CC)
// Allocate a pctPICTURE from a caller-filled pctHEADER: stamps the header with the pct
// signature, copies it into the new picture, allocates the per-face pointer table and one
// pixel buffer per face (all zero-filled). On any allocation failure the partially built
// picture is rolled back and nullptr returned.
// Source: D:\Projects\code\common\src.sys\picture\picture.cpp

#include "../../headers/pct_create_boundary.h"

static const char *const PCT_PICTURE_CPP =
    "D:\\Projects\\code\\common\\src.sys\\picture\\picture.cpp";

pctPICTURE *pctCreate(pctHEADER *pHdr)
{
    pctPICTURE *pic = (pctPICTURE *)operator new(sizeof(pctPICTURE), PCT_PICTURE_CPP, 0x62C);
    if (!pic)
        return nullptr;

    // Default-init the embedded header, then stamp the caller's header with the pct signature
    // and copy all seven header ints (sign/sx/sy/sz/nFaces/nMipMap/format) into the picture.
    pic->hdr.sign    = 0;
    pic->hdr.sx      = 0;
    pic->hdr.sy      = 0;
    pic->hdr.sz      = 1;
    pic->hdr.nFaces  = 1;
    pic->hdr.nMipMap = 1;
    pic->hdr.format  = 0;
    pic->pPalette    = nullptr;
    pic->pFaces      = nullptr;

    pHdr->sign = PCT_SIGN;
    pic->hdr   = *pHdr;

    pic->pFaces = (void **)dlCalloc(1u, 4 * pic->hdr.nFaces, PCT_PICTURE_CPP, 0x635);
    if (!pic->pFaces) {
        operator delete(pic);
        return nullptr;
    }

    unsigned int faceSize = (unsigned int)pctGetMipMapOffset(&pic->hdr, pic->hdr.nMipMap);
    // (The decompiler emits a second pctGetMipMapOffset call on a scratch header with sz forced
    //  to 1 whose result is discarded -- an inlined leftover/assert artifact; omitted here.)

    for (int face = 0; face < pic->hdr.nFaces; ++face) {
        pic->pFaces[face] = dlCalloc(1u, faceSize, PCT_PICTURE_CPP, 0x643);
        if (!pic->pFaces[face]) {
            for (int j = face - 1; j >= 0; --j)
                dlFree(pic->pFaces[j]);
            dlFree(pic->pFaces);
            operator delete(pic);
            return nullptr;
        }
    }
    return pic;
}
