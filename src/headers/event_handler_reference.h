#pragma once
/* event_handler_reference — one input-event → action binding in a ui_widget_definition
 * (ui_widget_definition.event_handler_references). 72-byte records. */

#include <stdint.h>
#include "tag_reference.h"

typedef struct event_handler_reference
{
    int           flags;             /* 0x00 */
    int16_t       event_type;        /* 0x04 */
    int16_t       function;          /* 0x06 */
    tag_reference ui_widget_tag;     /* 0x08 */
    tag_reference sound_effect_tag;  /* 0x18 */
    char          script_command[32];/* 0x28 */
} event_handler_reference;           /* 72 bytes */
