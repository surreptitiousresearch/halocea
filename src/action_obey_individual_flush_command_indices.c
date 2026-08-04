#include <stdint.h>
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/scenario.h"
#include "headers/ai_command_list_definition.h"
#include "headers/blam_data_globals.h"


void action_obey_individual_flush_command_indices(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, void *user_data)
{
    int command_count = ((ai_command_list_definition *)global_scenario->ai_command_lists.address)[command_list_index].commands.count;
    if ( simple_control->current_command_index >= command_count )
        simple_control->current_command_index = -1;
}
