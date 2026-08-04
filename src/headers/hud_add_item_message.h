#pragma once
/* hud_add_item_network_data — stateless message-delta payload for the "add HUD item message" network
 * message (8 bytes). DB-verified via types_members hud_add_item_network_data. */

typedef struct hud_add_item_network_data
{
    int             item_definition_index; /* 0x0 */
    unsigned __int8 message_offset;        /* 0x4 */
    unsigned char   _pad5;                 /* 0x5 */
    __int16         quantity;              /* 0x6 */
} hud_add_item_message; /* 0x8 bytes — legacy local name for hud_add_item_network_data */
