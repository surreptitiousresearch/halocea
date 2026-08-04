#include <stdint.h>
#include "headers/scenario.h"
#include "headers/ai_command_list_flags.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/unit_flags.h"
#include "headers/ai_command_list_definition.h"
#include "headers/blam_data_globals.h"


void action_obey_individual_begin(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, void *user_data)
{
    /* MIS-SCALED: decompiler *((_DWORD*)address + 24*index + 8) @837DED84 is DWORD-indexed,
     * i.e. element stride 96 (= ai_command_list_definition) and flags at byte 32 — not a 24-byte
     * stride with an +8 byte flags field as a prior byte-offset transcription assumed. */
    ai_command_list_definition *command_list =
        &((ai_command_list_definition *)global_scenario->ai_command_lists.address)[command_list_index];

    if ( command_list->flags & (1u << _ai_command_list_disable_falling_damage_bit) )   /* ai_command_list "no falling damage" flag */
    {
        unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        object->unit.flags |= (1u << _unit_no_falling_damage_bit);
    }
}
