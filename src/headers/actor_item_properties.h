#pragma once
/* actor_item_properties — actor_variant_definition.items (64 bytes), DB-verified
 * (types_members type_name='actor_item_properties'). Equipment/weapon-loadout block. */

#include <stdint.h>
#include "tag_reference.h"

typedef struct actor_item_properties
{
    tag_reference   equipment_reference;         /* 0x00 */
    int16_t         grenades_lower_bound;         /* 0x10 */
    int16_t         grenades_upper_bound;          /* 0x12 */
    float           dont_drop_grenades_chance;      /* 0x14 */
    float           weapon_loaded_lower_bound;       /* 0x18 */
    float           weapon_loaded_upper_bound;        /* 0x1C */
    int16_t         weapon_ammo_lower_bound;           /* 0x20 */
    int16_t         weapon_ammo_upper_bound;            /* 0x22 */
    unsigned int    weapon_unused[3];                    /* 0x24 */
    unsigned int    unused[4];                             /* 0x30 */
} actor_item_properties;                                   /* 0x40 = 64 bytes */
