#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/unit_flags.h"
#include "headers/obey_metadata_flags.h"
#include "headers/blam_data_globals.h"

extern void action_obey_command_end(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, uint8_t *next_command_index);

void action_obey_individual_end(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, void *user_data)
{
    unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unsigned __int8 next_command_index[16];

    if ( !(simple_control->metadata_flags & (1u << _obey_metadata_commands_finished_bit)) )
        action_obey_command_end(actor_index, unit_index, command_list_index, simple_control, complex_control, next_command_index);

    object->unit.flags &= ~(1u << _unit_no_falling_damage_bit);
}
