#pragma once
/* object_header_block_reference — a packed reference into the object header's variable-size block
 * region: a size/offset pair. Layout from the database (types_members). */

typedef struct object_header_block_reference
{
    __int16 size;      /* 0x00 */
    __int16 offset;    /* 0x02 */
} object_header_block_reference;   /* 4 bytes */
