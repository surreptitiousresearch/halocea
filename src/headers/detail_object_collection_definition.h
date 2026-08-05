#pragma once
/* detail_object_collection_definition — tag data for a detail-object palette collection. Layout from
 * the database. */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"

typedef struct detail_object_collection_definition
{
    int16_t          collection_type;  /* 0x00 */
    uint16_t pad;              /* 0x02 */
    float            global_z_offset;  /* 0x04 */
    int              unused1[11];      /* 0x08 */
    tag_reference    map;              /* 0x34 */
    tag_block        type_definitions; /* 0x44 */
    int              unused2[12];      /* 0x50 */
} detail_object_collection_definition; /* 128 bytes */
