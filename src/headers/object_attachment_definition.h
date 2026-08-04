#pragma once
/* object_attachment_definition — one element of an object definition's attachments tag block
 * (72 bytes). Layout from the database (types_members). */

#include "tag_reference.h"

typedef struct object_attachment_definition
{
    tag_reference    type;                               /* 0x00 */
    char             marker_name[32];                    /* 0x10 */
    __int16          primary_scale_function_reference;   /* 0x30 */
    __int16          secondary_scale_function_reference; /* 0x32 */
    __int16          change_color_reference;             /* 0x34 */
    unsigned __int16 pad;                                /* 0x36 */
    int              unused[4];                          /* 0x38 */
} object_attachment_definition;                          /* 72 bytes */
