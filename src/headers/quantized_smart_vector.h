#pragma once
/* quantized_smart_vector — a quantized 3-component vector (12 bytes, DB types_members-confirmed layout). */

typedef struct quantized_smart_vector
{
    unsigned int x; /* 0x0 */
    unsigned int y; /* 0x4 */
    unsigned int z; /* 0x8 */
} quantized_smart_vector; /* 12 bytes */
