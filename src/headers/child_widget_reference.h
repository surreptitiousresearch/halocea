#pragma once
/* child_widget_reference — one element of ui_widget_definition.child_widget_references
   (80 bytes; DB-verified). flags bit 1 = "use custom controller index". */

#include "tag_reference.h"

typedef struct child_widget_reference
{
    tag_reference ui_widget_tag;           /* 0x00 */
    char          name_unused[32];         /* 0x10 */
    int           flags;                   /* 0x30 */
    __int16       custom_controller_index; /* 0x34 */
    __int16       vertical_offset;         /* 0x36 */
    __int16       horizontal_offset;       /* 0x38 */
    __int16       pad;                     /* 0x3A */
    int           unused[5];               /* 0x3C */
} child_widget_reference;                  /* 80 bytes */
