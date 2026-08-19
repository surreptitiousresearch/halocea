/* action_obey_individual_perform @0x837DEBC0 — per-tick driver for an actor's "obey" command list
 * execution: unless the list has already finished (metadata_flags bit1), repeatedly performs the current
 * command, ends it, and begins the next one until either a command's "abort" flag (bit2) is set, a
 * command fails to begin, or the command index runs past the list's command count (marking the list
 * finished). If the list finishes without an abort, clears *user_data.
 *
 * The 96-byte ai_command_lists tag_block entry is ai_command_list_definition; the "command count"
 * (dword offset 48) is commands.count (commands tag_block @0x30). */

#include <stdint.h>
#include "headers/ai_command_list_definition.h"
#include "headers/scenario.h"
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/obey_metadata_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t action_obey_command_perform(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control);
extern void action_obey_command_end(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, uint8_t *next_command_index);
extern uint8_t action_obey_command_begin(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control);

void action_obey_individual_perform(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, uint8_t *user_data)
{
    const ai_command_list_definition_t *command_list =
        &((const ai_command_list_definition_t *)global_scenario->ai_command_lists.address)[command_list_index];

    if ( !(simple_control->metadata_flags & (1u << _obey_metadata_commands_finished_bit)) )
    {
        uint8_t command_valid = simple_control->current_command_index < command_list->commands.count;
        simple_control->loop_counter = 0;

        do
        {
            uint8_t command_was_valid = command_valid;

            if ( command_valid && !action_obey_command_perform(actor_index, unit_index, command_list_index,
                    simple_control, complex_control) )
                break;

            uint8_t next_command_index = simple_control->current_command_index + 1;

            if ( command_was_valid )
            {
                action_obey_command_end(actor_index, unit_index, command_list_index, simple_control,
                    complex_control, &next_command_index);
            }

            if ( next_command_index >= command_list->commands.count )
            {
                simple_control->metadata_flags |= (1u << _obey_metadata_commands_finished_bit);
                break;
            }

            simple_control->current_command_index = next_command_index;
            command_valid = action_obey_command_begin(actor_index, unit_index, command_list_index, simple_control,
                complex_control);
        }
        while ( !(simple_control->metadata_flags & (1u << _obey_metadata_action_changed_bit)) );
    }

    if ( !(simple_control->metadata_flags & (1u << _obey_metadata_commands_finished_bit)) )
        *user_data = 0;
}
