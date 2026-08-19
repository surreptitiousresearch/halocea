/* damage_data_new @0x836B13A0 — initialize a damage_data request to its defaults for the given damage tag.
 * Zeroes the structure, then sets the non-zero defaults: scale/multiplier = 1, owner/material indices = -1,
 * and an invalid (cluster_index -1) location. */

#include <string.h>
#include "headers/damage_data.h"


void damage_data_new(damage_data *damage_data, int definition_index)
{
    memset(damage_data, 0, sizeof(*damage_data));   /* decompiler wrote sizeof(pointer); real struct size */
    damage_data->definition_index = definition_index;
    damage_data->material_type = -1;
    damage_data->owner_player_index = -1;
    damage_data->owner_object_index = -1;
    damage_data->scale = 1.0f;
    damage_data->owner_team_index = -1;
    damage_data->multiplier = 1.0f;
    damage_data->location.cluster_index = -1;
}
