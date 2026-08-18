#pragma once

#include "d3d_render_boundary.h"  /* _D3DFORMAT */
#include "pctHEADER.h"            /* pctHEADER */
#include "pctPICTURE.h"           /* pctPICTURE */
#include "pctFORMAT.h"            /* pctFORMAT (pctGetFormat return type) */

extern pctPICTURE *pctCreate(int format, int width, int height, int mipmaps, int faces, int depth);
/* The pct* size/offset helpers below are reversed in src/ws/pct/. */
extern char *pctGetMipMapOffsetPtr(pctPICTURE *pic, int mipmap, int face);
extern int pctGetMemFaceSize(pctHEADER *hdr);
extern int pctGetMipMapMemSize(pctHEADER *hdr, int mipmap); /* DEVIATION: char->int per mangling ?pctGetMipMapMemSize@@YAHPAUpctHEADER@@H@Z */
extern int pctGetMipMapOffset(pctHEADER *hdr, int mipmap);
/* pctGetFormat is an OVERLOAD SET of five in the binary (names table); the two used here:
   ?pctGetFormat@@YAPAUpctFORMAT@@H@Z @0x827BFC60 (by format code) and
   ?pctGetFormat@@YAPAUpctFORMAT@@PAUpctPICTURE@@@Z @0x827C0AB8 (by picture — what the
   hcex_tex/hcex_set_tex call sites pass: `mr r3,pPict`). The earlier one-declaration
   "corrections" in each direction were both flattening this set to one signature. */
extern pctFORMAT *pctGetFormat(int format);
extern pctFORMAT *pctGetFormat(pctPICTURE *pic);
extern void pctDestroy(pctPICTURE *pic);
/* Deep-copy a picture (allocates and returns a new pctPICTURE). boundary. */
extern pctPICTURE *pctDuplicate(pctPICTURE *pic);

/* Total memory size (all faces/mips) for a filled-out header, and a query for
   whether a pct picture format is block-compressed (DXTn). */
extern int pctGetMemSize(pctHEADER *hdr);
extern unsigned int pctIsCompressed(unsigned int format);
