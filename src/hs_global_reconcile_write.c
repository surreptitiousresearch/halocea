/* hs_global_reconcile_write @0x8368DF10 — write an external (engine-defined) HaloScript global's cached value
 * back to its backing engine variable, so an engine read sees what a script just set. Only externals
 * (HS_GLOBAL_EXTERNAL_FLAG bit) are reconciled; scenario globals are stored directly. The cached 8-byte slot
 * holds the value at +4; the
 * store width is chosen by type (boolean byte / real float / word index-and-enum types / dword for long,
 * string, real-float-vector and object types). A null backing pointer is skipped.
 *
 * The type case groups are reproduced from the database switch (grouped by store width, matching the original).
 * The counterpart reader is hs_global_reconcile_read. */

#include <stdint.h>
#include "headers/hs_global_designator.h"
#include "headers/hs_global_external.h"
#include "headers/hs_global_datum.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int16_t hs_global_get_type(int16_t designator);

void hs_global_reconcile_write(int16_t global_designator)
{
    if ( !HS_GLOBAL_IS_EXTERNAL(global_designator) )
        return;

    hs_global_external *descriptor = hs_external_globals[HS_GLOBAL_DESIGNATOR_TO_INDEX(global_designator)];
    hs_global_datum *slot = DATA_ARRAY_ELEMENT(hs_global_data, hs_global_datum, global_designator);
    void *backing = descriptor->pointer; /* recovered: descriptor->address -> pointer (member at 0x08) */

    switch ( hs_global_get_type(global_designator) )
    {
        case hs_type_boolean:
            if ( backing )
                *(unsigned char *)backing = slot->value.boolean;
            break;
        case hs_type_real:
            if ( backing )
                *(float *)backing = slot->value.real;
            break;
        case hs_type_short_integer: case hs_type_script: case hs_type_trigger_volume:
        case hs_type_cutscene_flag: case hs_type_cutscene_camera_point: case hs_type_cutscene_title:
        case hs_type_cutscene_recording: case hs_type_device_group:
        case hs_type_ai_command_list: case hs_type_starting_profile: case hs_type_conversation:
        case hs_type_navpoint: case hs_type_hud_message:
        case hs_type_enum_game_difficulty: case hs_type_enum_team: case hs_type_enum_ai_default_state:
        case hs_type_enum_actor_type: case hs_type_enum_hud_corner:
        case hs_type_object_name:   /* word index / enum / object-name types */
            if ( backing )
                *(int16_t *)backing = slot->value.short_value;
            break;
        case hs_type_long_integer: case hs_type_string: case hs_type_ai:
        case hs_type_object_list: case hs_type_sound: case hs_type_effect: case hs_type_damage:
        case hs_type_looping_sound: case hs_type_animation_graph: case hs_type_actor_variant:
        case hs_type_damage_effect: case hs_type_object_definition:
        case hs_type_object: case hs_type_unit: case hs_type_vehicle: case hs_type_weapon:
        case hs_type_device: case hs_type_scenery:   /* long / string / real-vector / object types */
            if ( backing )
                *(int *)backing = slot->value.long_value;
            break;
        default:
            return;
    }
}
