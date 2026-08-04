#pragma once

/* 2D rectangle bounds (8 bytes). DB union: flat n[4] view plus the anonymous
 * named-edge struct view ($-name kept verbatim; $ in identifiers is an MSVC
 * extension). Note the field order: y0, x0, y1, x1. */

/* DB struct _A24E1B84F937764C50C815BEE76035B3 — named-edge view. */
typedef struct _A24E1B84F937764C50C815BEE76035B3
{
    short y0; /* 0x00 — n[0] */
    short x0; /* 0x02 — n[1] */
    short y1; /* 0x04 — n[2] */
    short x1; /* 0x06 — n[3] */
} _A24E1B84F937764C50C815BEE76035B3;

typedef union rectangle2d
{
    short n[4];                                    /* 0x00 */
    struct _A24E1B84F937764C50C815BEE76035B3 __s1; /* 0x00 — named-edge view */
    struct { short y0, x0, y1, x1; };              /* 0x00 — direct view (order y0,x0,y1,x1) */
} rectangle2d; /* 8 bytes */
