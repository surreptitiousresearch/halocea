#pragma once

/* real_rectangle2d — float 2D rectangle (16 bytes); indexed view n[4]. DB
 * union: flat n[4] view plus the named x0/x1/y0/y1 anonymous struct view
 * ($-name kept verbatim; $ in identifiers is an MSVC extension). */

/* DB struct _7789E25AA435C414CB4DE2C745EAF104 — x0/x1/y0/y1 view. */
typedef struct _7789E25AA435C414CB4DE2C745EAF104
{
    float x0; /* 0x00 */
    float x1; /* 0x04 */
    float y0; /* 0x08 */
    float y1; /* 0x0C */
} _7789E25AA435C414CB4DE2C745EAF104;

typedef union real_rectangle2d
{
    float n[4];                                    /* 0x00 */
    struct _7789E25AA435C414CB4DE2C745EAF104 __s1; /* 0x00 — named view */
    struct { float x0, x1, y0, y1; };              /* 0x00 — direct view (consumers use rect.x0 etc.) */
} real_rectangle2d; /* 16 bytes */
