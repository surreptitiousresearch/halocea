#pragma once
/* tag_data — variable-size tag data block reference (tag definitions) */

typedef struct tag_data_definition tag_data_definition;

typedef struct tag_data
{
    int                  size;        /* 0x00 */
    unsigned int         pad;         /* 0x04 */
    int                  file_offset; /* 0x08 */
    void                *address;     /* 0x0C */
    tag_data_definition *definition;  /* 0x10 */
} tag_data;                           /* 20 bytes */
