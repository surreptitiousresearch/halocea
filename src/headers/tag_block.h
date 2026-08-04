#pragma once
/* tag_block — variable-length tag array reference (tag definitions) */

typedef struct tag_block_definition tag_block_definition;

typedef struct tag_block
{
    int                   count;      /* 0x0 */
    void                 *address;    /* 0x4 */
    tag_block_definition *definition; /* 0x8 */
} tag_block;                          /* 12 bytes */
