#pragma once

/* Per-format descriptor for the pct picture/texture subsystem. One entry exists
 * per PCT_FORMAT code in the global table `pctFormatList`. Layout DB-verified via
 * types_members pctFORMAT (size 116). Source: D:\Projects\code\common\src.sys\
 * picture\picture.cpp. */

typedef struct pctFORMAT
{
    int          nChannel;  /* 0x00 number of channels */
    int          state;     /* 0x04 format state flags (bit 3 = block-compressed) */
    int          bpp;       /* 0x08 bytes per pixel (0 for block-compressed) */
    int          bitpp;     /* 0x0C bits per pixel */
    unsigned int mask[4];   /* 0x10 per-channel bit masks (A,R,G,B) */
    int          shift[4];  /* 0x20 per-channel bit shifts */
    char         length[4]; /* 0x30 per-channel bit lengths */
    char         name[64];  /* 0x34 human-readable format name */
} pctFORMAT;                /* 116 bytes */

/* ?_pctFormatList / pctFormatList — global format-descriptor table indexed by
 * PCT_FORMAT code (0 .. PCT_FORMAT_MAX-1). Boundary data object. */
extern pctFORMAT pctFormatList[];
