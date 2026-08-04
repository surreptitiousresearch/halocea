#pragma once
/* object_list_header_datum — an element of the object_list header data_array: reference count,
 * element count, and head of the reference chain. Layout from the database (types_members). */

typedef struct object_list_header_datum
{
    __int16          identifier;             /* 0x0 */
    unsigned __int16 pad;                    /* 0x2 */
    __int16          reference_count;        /* 0x4 */
    __int16          count;                  /* 0x6 */
    int              first_reference_index;  /* 0x8 */
} object_list_header_datum;                  /* 12 bytes */
