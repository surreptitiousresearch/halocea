#pragma once
/* conditional_widget_reference — a ui_widget_definition's fallback-launch entry
 * (conditional_widget_references tag_block, 80-byte stride). DB-verified layout. */

#include "tag_reference.h"

typedef struct conditional_widget_reference
{
    tag_reference ui_widget_tag;        /* 0x00: index at +12 */
    char name_unused[32];               /* 0x10 */
    int flags;                          /* 0x30 */
    __int16 custom_controller_index;    /* 0x34 */
    __int16 pad;                        /* 0x36 */
    int unused[6];                      /* 0x38 */
} conditional_widget_reference;
