#pragma once
/* _widget_stack_node_data — 12-byte saved-state block for a widget stack node. */

typedef struct _widget_stack_node_data
{
    int     previous_widget_tag;             /* 0x0 */
    int     focused_child_parent_widget_tag; /* 0x4 */
    __int16 focused_child_index;             /* 0x8 */
    __int16 local_player_index;              /* 0xA */
} _widget_stack_node_data;                   /* 12 bytes */
