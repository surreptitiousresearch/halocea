/* hs_global_reconcile_read @0x8368D8F8 — refresh an external (engine-defined) HaloScript global's cached value
 * in hs_global_data from its backing engine variable, so a subsequent read sees the live value. Only externals
 * (HS_GLOBAL_EXTERNAL_FLAG bit) are reconciled; scenario globals are stored directly and need no
 * reconciliation. The cached
 * 8-byte slot holds the value at offset +4; the read is typed (boolean byte / real float / index word / string
 * dword) and falls back to a per-type default when the backing pointer is null.
 *
 * The switch and per-type defaults are reproduced verbatim from the database (cases share shapes but keep
 * distinct default symbols, so they are not merged). */

#include <stdint.h>
#include "headers/hs_global_designator.h"
#include "headers/hs_global_external.h"
#include "headers/hs_global_datum.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include <math.h>   /* NAN */
#include "headers/blam_data_globals.h"

extern int16_t hs_global_get_type(int16_t designator);

extern const unsigned char hs_type_boolean_default;
extern const int16_t hs_type_short_integer_default;
extern const int16_t hs_type_script_default;
extern const int16_t hs_type_trigger_volume_default;
extern const int16_t hs_type_cutscene_flag_default;
extern const int16_t hs_type_cutscene_camera_point_default;
extern const int16_t hs_type_cutscene_title_default;
extern const int16_t hs_type_cutscene_recording_default;
extern const int16_t hs_type_device_group_default;
extern const int16_t hs_type_ai_command_list_default;
extern const int16_t hs_type_starting_profile_default;
extern const int16_t hs_type_conversation_default;
extern const int16_t hs_type_navpoint_default;
extern const int16_t hs_type_hud_message_default;
extern const int16_t hs_type_enum_game_difficulty_default;
extern const int16_t hs_type_enum_team_default;
extern const int16_t hs_type_enum_ai_default_state_default;
extern const int16_t hs_type_enum_actor_type_default;
extern const int16_t hs_type_enum_hud_corner_default;
extern const int16_t hs_type_object_name_default;

void hs_global_reconcile_read(int16_t global_designator)
{
    if ( !HS_GLOBAL_IS_EXTERNAL(global_designator) )
        return;

    hs_global_external *descriptor = hs_external_globals[HS_GLOBAL_DESIGNATOR_TO_INDEX(global_designator)];
    hs_global_datum *slot = DATA_ARRAY_ELEMENT(hs_global_data, hs_global_datum, global_designator);
    void *backing = descriptor->pointer; /* recovered: descriptor->address -> pointer (member at 0x08) */

    int16_t word_default;
    switch ( hs_global_get_type(global_designator) )
    {
        case hs_type_boolean:
            slot->value.boolean = backing ? *(unsigned char *)backing : hs_type_boolean_default;
            return;
        case hs_type_real:
            slot->value.real = backing ? *(float *)backing : 0.0f;
            return;
        case hs_type_short_integer:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_short_integer_default; break;
        case hs_type_long_integer:     /* long integer (read as real in the original) */
            slot->value.real = backing ? *(float *)backing : 0.0f;
            return;
        case hs_type_string:
            if ( backing ) slot->value.real = *(float *)backing;
            else slot->value.long_value = (int)hs_type_string_default;
            return;
        case hs_type_script:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_script_default; break;
        case hs_type_trigger_volume:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_trigger_volume_default; break;
        case hs_type_cutscene_flag:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_cutscene_flag_default; break;
        case hs_type_cutscene_camera_point:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_cutscene_camera_point_default; break;
        case hs_type_cutscene_title:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_cutscene_title_default; break;
        case hs_type_cutscene_recording:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_cutscene_recording_default; break;
        case hs_type_device_group:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_device_group_default; break;
        case hs_type_ai:    /* real (NAN default) */
            slot->value.real = backing ? *(float *)backing : NAN;
            return;
        case hs_type_ai_command_list:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_ai_command_list_default; break;
        case hs_type_starting_profile:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_starting_profile_default; break;
        case hs_type_conversation:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_conversation_default; break;
        case hs_type_navpoint:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_navpoint_default; break;
        case hs_type_hud_message:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_hud_message_default; break;
        case hs_type_object_list: case hs_type_sound: case hs_type_effect: case hs_type_damage:
        case hs_type_looping_sound: case hs_type_animation_graph: case hs_type_actor_variant:
        case hs_type_damage_effect: case hs_type_object_definition:
        case hs_type_object: case hs_type_unit: case hs_type_vehicle: case hs_type_weapon:
        case hs_type_device: case hs_type_scenery:   /* real / vector components */
            slot->value.real = backing ? *(float *)backing : NAN;
            return;
        case hs_type_enum_game_difficulty:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_enum_game_difficulty_default; break;
        case hs_type_enum_team:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_enum_team_default; break;
        case hs_type_enum_ai_default_state:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_enum_ai_default_state_default; break;
        case hs_type_enum_actor_type:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_enum_actor_type_default; break;
        case hs_type_enum_hud_corner:
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_enum_hud_corner_default; break;
        case hs_type_object_name:    /* object name */
            if ( backing ) { slot->value.short_value = *(int16_t *)backing; return; }
            word_default = hs_type_object_name_default; break;
        default:
            return;
    }
    slot->value.short_value = word_default;
}
