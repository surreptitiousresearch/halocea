#pragma once
/* object_attachment_definition — one element of an object definition's attachments tag block
 * (72 bytes). Layout from the database (types_members). */

#include <stdint.h>
#include "tag_reference.h"

typedef struct object_attachment_definition
{
    tag_reference    type;                               /* 0x00 */
    char             marker_name[32];                    /* 0x10 */
    int16_t          primary_scale_function_reference;   /* 0x30 */
    int16_t          secondary_scale_function_reference; /* 0x32 */
    int16_t          change_color_reference;             /* 0x34 */
    uint16_t pad;                                /* 0x36 */
    int              unused[4];                          /* 0x38 */
} object_attachment_definition;                          /* 72 bytes */
