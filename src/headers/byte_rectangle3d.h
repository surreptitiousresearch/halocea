#pragma once
/* byte_rectangle3d — a 6-byte quantized AABB (one byte per min/max per axis), dequantized against a parent
 * real_rectangle3d. Used for compact BSP3D node bounds. Layout from the database (6 bytes, union): a raw
 * n[6] view overlaid with the anonymous per-axis struct _69587EA0C1F45178AB44DB574ECC7871, named __s1.
 * $-name kept verbatim ($ in identifiers is an MSVC extension), matching the mtlPARAM.h convention. */

/* DB anonymous struct _69587EA0C1F45178AB44DB574ECC7871 — per-axis min/max bytes */
typedef struct _69587EA0C1F45178AB44DB574ECC7871
{
    unsigned __int8 x0;               /* 0x00 */
    unsigned __int8 x1;               /* 0x01 */
    unsigned __int8 y0;               /* 0x02 */
    unsigned __int8 y1;               /* 0x03 */
    unsigned __int8 z0;               /* 0x04 */
    unsigned __int8 z1;               /* 0x05 */
} _69587EA0C1F45178AB44DB574ECC7871;

typedef union byte_rectangle3d
{
    unsigned __int8 n[6];                     /* 0x00 */
    struct _69587EA0C1F45178AB44DB574ECC7871 __s1; /* 0x00 */
} byte_rectangle3d; /* 6 bytes */
