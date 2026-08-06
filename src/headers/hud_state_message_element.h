#pragma once
/* hud_state_message_element — one element record of a hud_state_messages tag's elements block
 * (2 bytes; DB-verified, matches headers_ref sizeof=0x2). A hud_state_message_definition names a
 * run of these via element_start_index / element_count. */

#include <stdint.h>

typedef struct hud_state_message_element
{
    uint8_t type;    /* 0x00 */
    uint8_t data;    /* 0x01 */
} hud_state_message_element;   /* 2 bytes */
