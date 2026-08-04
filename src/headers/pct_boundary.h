#pragma once

#include "d3d_render_boundary.h"  /* _D3DFORMAT */
#include "pctHEADER.h"            /* pctHEADER */
#include "pctPICTURE.h"           /* pctPICTURE */
#include "pctFORMAT.h"            /* pctFORMAT (pctGetFormat return type) */

extern pctPICTURE *pctCreate(int format, int width, int height, int mipmaps, int faces, int depth);
/* The pct* size/offset helpers below are reversed in src/ws/pct/. */
extern char *pctGetMipMapOffsetPtr(pctPICTURE *pic, int mipmap, int face);
extern int pctGetMemFaceSize(pctHEADER *hdr);
extern int pctGetMipMapMemSize(pctHEADER *hdr, char mipmap);
extern int pctGetMipMapOffset(pctHEADER *hdr, int mipmap);
/* Returns a pointer into the global pctFormatList table for the given PCT_FORMAT
   code (NOT a query on a picture). The former `int pctGetFormat(pctPICTURE*)`
   prototype was wrong — corrected against the DB signature. */
extern pctFORMAT *pctGetFormat(unsigned int format);
extern void pctDestroy(pctPICTURE *pic);
/* Deep-copy a picture (allocates and returns a new pctPICTURE). boundary. */
extern pctPICTURE *pctDuplicate(pctPICTURE *pic);

/* Total memory size (all faces/mips) for a filled-out header, and a query for
   whether a pct picture format is block-compressed (DXTn). */
extern int pctGetMemSize(pctHEADER *hdr);
extern unsigned int pctIsCompressed(unsigned int format);
