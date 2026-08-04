#pragma once

/* Boundary surface for the pct picture allocator functions (pctCreate / pctDuplicate),
 * reversed in src/ws/pct/. Only the low-level allocator + the pct signature magic and the
 * mip-offset helper are needed here; the pct picture/header/format types come from their
 * own headers. */

#include <cstddef>
#include "pctHEADER.h"
#include "pctPICTURE.h"

/* pctPICTURE::hdr.sign magic stamped on every picture created by pctCreate. 'PICT' big-endian
 * (0x50494354 == 1346978644), verbatim from the decompiler constant. */
#define PCT_SIGN 0x50494354u

/* Debug/placement global allocator the ws build links in: operator new(size, file, line) and the
 * matching operator delete. boundary (XDK/dl heap). */
void *operator new(size_t size, const char *file, int line);
void  operator delete(void *p);

/* dl heap primitives. boundary. */
extern "C" void *dlCalloc(unsigned int num, unsigned int size, const char *file, unsigned int line);
extern "C" void dlFree(void *ptr);

/* Byte offset (== total size) of mip level `mipmap` within one face of a picture with the given
 * header. Reversed in src/ws/pct/pctGetMipMapOffset.cpp. */
extern int pctGetMipMapOffset(pctHEADER *hdr, int mipmap);

/* Core allocator: build a picture from a filled-out header. DB: pctPICTURE *pctCreate(pctHEADER *).
 * The 6-int convenience overload is reversed in pctCreate_int6.cpp. */
extern pctPICTURE *pctCreate(pctHEADER *pHdr);
