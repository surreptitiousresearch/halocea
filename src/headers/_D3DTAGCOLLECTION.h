#pragma once
#include <stdint.h>
/* _D3DTAGCOLLECTION — Xbox 360 D3D resource-tag bitmask collection (external library boundary
 * type; DB types_members _D3DTAGCOLLECTION, 40 bytes). */

typedef struct _D3DTAGCOLLECTION
{
    uint64_t m_Mask[5]; /* 0x00 */
} _D3DTAGCOLLECTION; /* 40 bytes */
