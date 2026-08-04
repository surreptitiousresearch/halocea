#pragma once
/* damage_dealt_network_data — packet payload accumulating damage dealt to an object, sent to the network
 * scoring system. Layout from the database (12 bytes). */

typedef struct damage_dealt_network_data
{
    int             damaged_object_index;  /* 0x00 */
    float           shield_damage;         /* 0x04 */
    unsigned __int8 shields_depleted;      /* 0x08 */
} damage_dealt_network_data;               /* 0x0C (12 bytes) */
