#pragma once
#include <stdint.h>
/* rgb_color — 16-bit-per-channel color (6 bytes). DB union: flat n[3] view
 * plus the anonymous red/green/blue struct view ($-name kept verbatim; $ in
 * identifiers is an MSVC extension). */

/* DB struct _EBE355A101A506B8BB9BC1A4B6A6F5B0 — red/green/blue view. */
typedef struct _EBE355A101A506B8BB9BC1A4B6A6F5B0
{
    uint16_t red;   /* 0x00 */
    uint16_t green; /* 0x02 */
    uint16_t blue;  /* 0x04 */
} _EBE355A101A506B8BB9BC1A4B6A6F5B0;

typedef union rgb_color
{
    uint16_t n[3];                         /* 0x00 */
    struct _EBE355A101A506B8BB9BC1A4B6A6F5B0 __s1; /* 0x00 — red/green/blue view */
} rgb_color; /* 6 bytes */
