#pragma once
/* object_definition_widget — one entry in an object definition's widgets tag_block (32 bytes).
 * DB-verified layout (types_members object_definition_widget): the widget's type tag reference. */

#include "tag_reference.h"

typedef struct object_definition_widget
{
    tag_reference type;       /* 0x00 */
    int           unused[4];  /* 0x10 */
} object_definition_widget;   /* 32 bytes */
