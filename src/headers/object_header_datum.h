#pragma once
/* object_header_datum — an element of the object header data_array: datum id, flags/type bytes,
 * cluster index, allocation size, and pointer to the live object_datum. Layout from the database
 * (types_members). */

#include <stdint.h>
#include "object_datum.h"

typedef struct object_header_datum
{
    int16_t          identifier;     /* 0x0 */
    uint8_t  flags;          /* 0x2 */
    uint8_t  type;           /* 0x3 */
    int16_t          cluster_index;  /* 0x4 */
    int16_t          data_size;      /* 0x6 */
    object_datum    *datum;          /* 0x8 */
} object_header_datum;               /* 12 bytes */
