#pragma once
#include <stdint.h>
/* object_list_header_datum — an element of the object_list header data_array: reference count,
 * element count, and head of the reference chain. Layout from the database (types_members). */

typedef struct object_list_header_datum
{
    int16_t          identifier;             /* 0x0 */
    uint16_t pad;                    /* 0x2 */
    int16_t          reference_count;        /* 0x4 */
    int16_t          count;                  /* 0x6 */
    int              first_reference_index;  /* 0x8 */
} object_list_header_datum;                  /* 12 bytes */
