/* action_obey_command_end @0x837DE8E8 — clean up the effects of the AI "obey" command that is ending, so the
 * next command starts from a neutral control state. Looks up the current command in the actor's obey command
 * list; based on the command opcode it halts movement (move/goto commands, only if the obeying unit is the
 * actor's own unit) and clears the complex-control destination, releases a direct-facing hold, stops secondary
 * looking (look commands), clears the shoot-at-target flag, or clears the obeying biped's absolute-movement/no-collision bits. The loop
 * command (0x14) optionally jumps the next-command cursor back to its loop target: unconditionally for a plain
 * loop, or gated on a metadata flag toggle for a conditional loop, capped at 10 iterations via loop_counter.
 * Out-of-range command indices and unhandled opcodes are no-ops.
 *
 * Command-list records are 96 bytes (count @+48, commands pointer @+52); each command is 32 bytes (opcode @0,
 * argument @+2, loop target @+22). Actor is the 1828-byte actor_datum. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/ai_command_list_definition.h"
#include "headers/ai_command_definition.h"
#include "headers/actor_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_datum_flags.h"
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_simple_control_flags.h"
#include "headers/obey_metadata_flags.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/ai_atom_type.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_move_halt(uint16_t actor_index);
extern void actor_look_secondary_stop(uint16_t actor_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void action_obey_command_end(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, uint8_t *next_command_index)
{
    ai_command_list_definition_t *command_list =
        &((ai_command_list_definition_t *)global_scenario->ai_command_lists.address)[command_list_index];
    int current_command_index = simple_control->current_command_index;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( current_command_index >= command_list->commands.count )
        return;

    ai_command_definition *command =
        &((ai_command_definition *)command_list->commands.address)[current_command_index];
    switch ( command->atom_type )
    {
        case _ai_atom_go_to:
        case _ai_atom_go_to_and_face:
            if ( unit_index == actor->meta.unit_index )
                actor_move_halt(actor_index);
            if ( complex_control )
            {
                complex_control->destination_valid = 0;
                complex_control->destination_facing = 0;
            }
            break;

        case _ai_atom_move_direction:
        case _ai_atom_move_immediate:
            simple_control->___u5.directmovement.facing = -1;
            simple_control->simple_control_flags &= ~(1u << _obey_simple_directmovement_bit);
            break;

        case _ai_atom_look:
        case _ai_atom_look_random:
        case _ai_atom_look_player:
        case _ai_atom_look_object:
            if ( unit_index == actor->meta.unit_index )
                actor_look_secondary_stop(actor_index);
            break;

        case _ai_atom_shoot:
            if ( complex_control )
                complex_control->shoot_at_target = 0;
            break;

        case _ai_atom_running_jump:
        case _ai_atom_targeted_jump:
            simple_control->___u5.directmovement.facing = 0;
            simple_control->simple_control_flags &= ~(1u << _obey_simple_jump_bit);
            break;

        case _ai_atom_animate:
        {
            /* DEVIATION: decompiler mistyped the object_mask_biped lookup as vehicle_datum; the 32-bit
             * RMW at 0x4CC (lwz/rlwinm 0,30,27/stw @0x837DEB00) is biped_datum.biped.flags (DB: uint32
             * at 0x4CC), clearing the absolute_movement + no_collision bits. */
            biped_datum *biped = object_try_and_get_and_verify_type(unit_index, object_mask_biped);
            if ( biped )
                biped->biped.flags &= ~((1u << _biped_absolute_movement_bit) | (1u << _biped_no_collision_bit));
            break;
        }

        case _ai_atom_loop:
        {
            uint8_t should_jump = 1;
            if ( command->atom_modifier == 1 )
            {
                uint8_t cleared_flags = simple_control->metadata_flags & ~(1u << _obey_metadata_told_to_advance_bit);
                should_jump = (~simple_control->metadata_flags >> _obey_metadata_told_to_advance_bit) & 1;
                simple_control->metadata_flags = cleared_flags;
                if ( should_jump )
                    simple_control->metadata_flags = cleared_flags | (1u << _obey_metadata_waiting_for_advance_notification_bit);
                else
                    simple_control->metadata_flags = cleared_flags & ~(1u << _obey_metadata_waiting_for_advance_notification_bit);
            }
            if ( should_jump )
            {
                int16_t loop_target = command->command_index;
                if ( loop_target != current_command_index && simple_control->loop_counter < 0xAu )
                {
                    *next_command_index = (uint8_t)loop_target;
                    ++simple_control->loop_counter;
                }
            }
            break;
        }

        default:
            return;
    }
}
