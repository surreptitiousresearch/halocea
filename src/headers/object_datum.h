#pragma once
/* object_datum — a live object instance datum stored in object_header_data's pool. A 4-byte definition
 * index followed by the 496-byte runtime payload (_object_datum, defined in its own header). Layout from
 * the database (types_members). */

#include "_object_datum.h"

typedef struct object_datum
{
    int           definition_index;   /* 0x00 */
    _object_datum object;             /* 0x04 */
} object_datum;                       /* 500 bytes */
