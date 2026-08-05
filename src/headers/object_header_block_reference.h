#pragma once
#include <stdint.h>
/* object_header_block_reference — a packed reference into the object header's variable-size block
 * region: a size/offset pair. Layout from the database (types_members). */

typedef struct object_header_block_reference
{
    int16_t size;      /* 0x00 */
    int16_t offset;    /* 0x02 */
} object_header_block_reference;   /* 4 bytes */
