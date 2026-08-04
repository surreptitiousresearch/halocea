#pragma once
/* widget_stack_node — node of a widget stack: a 12-byte saved-state block followed by the next pointer. */

#include "_widget_stack_node_data.h"

typedef struct widget_stack_node
{
    _widget_stack_node_data   data; /* 0x0 */
    struct widget_stack_node *next; /* 0xC */
} widget_stack_node;                /* 16 bytes */
