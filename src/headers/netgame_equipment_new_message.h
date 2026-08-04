#pragma once
/* netgame_equipment_new_network_data — the payload encoded for the _message_netgame_equipment_new
 * message delta when the server spawns a netgame-equipment item. Built on the stack and handed to
 * message_delta_processor_encode_stateless.
 * DB-verified via types_members netgame_equipment_new_network_data (12 bytes). */

typedef struct netgame_equipment_new_network_data
{
    int     server_object_index;     /* 0x00 — networked object-index field translation */
    int     item_definition_index;   /* 0x04 — object's item definition tag index */
    __int16 netgame_equipment_index; /* 0x08 — the scenario netgame-equipment index */
} netgame_equipment_new_message; /* legacy local name for netgame_equipment_new_network_data */
