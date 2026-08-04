/* create_the_flag_at_position @0x83805F20 — spawn the CTF flag object at a position for a team. */

#include <stdint.h>
#include "headers/object_placement_data.h"
#include "headers/real_point3d.h"

extern int get_flag_definition_index(void);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);

int create_the_flag_at_position(real_point3d *position, int team_index)
{
    object_placement_data data;

    object_placement_data_new(&data, get_flag_definition_index(), -1);
    data.owner_team_index = team_index;
    data.position = *position;

    int object_index = object_new(&data);
    object_set_automatic_deactivation(object_index, 0);
    return object_index;
}
