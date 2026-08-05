#pragma once
#include <stdint.h>
/* hsv_color — 16-bit-per-channel HSV color, DB layout (6 bytes, union). n[0]=hue, n[1]=saturation, n[2]=value.
 * Hue is stored as a full turn scaled across the 16-bit range (value*65536); saturation and value use
 * value*65535. Sibling of rgb_color.h. DB union: flat n[3] view plus the named
 * anonymous struct view ($-name kept verbatim; $ in identifiers is an MSVC extension). */

/* DB struct _DC8FB47B83C0ADBA0E53690C2F43F69C — hue/saturation/value view. */
typedef struct _DC8FB47B83C0ADBA0E53690C2F43F69C
{
    uint16_t hue;        /* 0x00 */
    uint16_t saturation; /* 0x02 */
    uint16_t value;      /* 0x04 */
} _DC8FB47B83C0ADBA0E53690C2F43F69C;

typedef union hsv_color
{
    uint16_t n[3];                         /* 0x0 — n[0]=hue, n[1]=saturation, n[2]=value */
    struct _DC8FB47B83C0ADBA0E53690C2F43F69C __s1; /* 0x00 — named view */
} hsv_color; /* 6 bytes */
