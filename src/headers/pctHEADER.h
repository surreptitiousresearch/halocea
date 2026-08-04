#pragma once

/* Picture/texture-format subsystem (pct*) boundary — not decompiled. Layout
   DB-verified via types_members pctHEADER (28 bytes). */
typedef struct pctHEADER
{
    unsigned int sign;    /* 0x00 */
    int          sx;      /* 0x04 width */
    int          sy;      /* 0x08 height */
    int          sz;      /* 0x0C depth */
    int          nFaces;  /* 0x10 */
    int          nMipMap; /* 0x14 */
    int          format;  /* 0x18 pct picture format */
} pctHEADER;              /* 28 bytes */
