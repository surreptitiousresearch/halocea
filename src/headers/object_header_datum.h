#pragma once
/* object_header_datum — an element of the object header data_array: datum id, flags/type bytes,
 * cluster index, allocation size, and pointer to the live object_datum. Layout from the database
 * (types_members). */

#include "object_datum.h"

typedef struct object_header_datum
{
    __int16          identifier;     /* 0x0 */
    unsigned __int8  flags;          /* 0x2 */
    unsigned __int8  type;           /* 0x3 */
    __int16          cluster_index;  /* 0x4 */
    __int16          data_size;      /* 0x6 */
    object_datum    *datum;          /* 0x8 */
} object_header_datum;               /* 12 bytes */
