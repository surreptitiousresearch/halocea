/* hs_global_reconcile_read @0x8368D8F8 — refresh an external (engine-defined) HaloScript global's cached value
 * in hs_global_data from its backing engine variable, so a subsequent read sees the live value. Only externals
 * (HS_GLOBAL_EXTERNAL_FLAG bit) are reconciled; scenario globals are stored directly and need no
 * reconciliation. The cached
 * 8-byte slot holds the value at offset +4; the read is typed (boolean byte / real float / index-and-enum word /
 * long, string, ai, tag-reference and object dword) and falls back to a per-type default when the backing
 * pointer is null.
 *
 * The switch is a 39-entry jump table (jpt_8368D964, switch value = type - hs_type_boolean, verified by decoding
 * the table bytes at 0x8368D968). EVERY case loads its own NAMED default global — no two cases share one — so
 * the cases are reproduced one-for-one and are deliberately not merged. The counterpart writer is
 * hs_global_reconcile_write, whose *(int *) grouping matches the dword cases below.
 *
 * DEVIATION: the whole tag-reference/object family (ai, object_list, sound, effect, damage, looping_sound,
 * animation_graph, actor_variant, damage_effect, object_definition, object, unit, vehicle, weapon, device,
 * scenery) previously collapsed onto `slot->value.real = backing ? *(float *)backing : NAN`. The binary reads
 * the backing with lwz and stores with stw (e.g. case 21/damage @0x8368DC84-0x8368DCA8), and each null branch
 * loads a distinct named default whose image value is 0xFFFFFFFF — the -1 "no tag" sentinel. C's NAN is
 * 0x7FC00000, a different bit pattern, which defeats every raw -1 compare downstream.
 *
 * DEVIATION: case 3 / hs_type_long_integer @0x8368DA74 was read as `*(float *)` with a 0.0f default and carried
 * a comment claiming it was "read as real in the original". The disassembly refutes that: the backing load is
 * `lwz r11, 0(r11)` @0x8368DA80 and the default load is `lwz r11, _hs_type_long_integer_default@l(r11)`
 * @0x8368DA90 (opcode 0x81, an integer load) — both integer, stored with stw. Value-equal, wrong width class.
 *
 * DEVIATION: case 4 / hs_type_string @0x8368DA9C read its backing with `*(float *)`; the binary uses
 * lwz @0x8368DAA8 / stw @0x8368DAAC. Only the backing branch was wrong; the default branch was already correct.
 *
 * DEVIATION: case 1 / hs_type_real @0x8368DA2C used a literal 0.0f default. The binary loads the named
 * hs_type_real_default with lfs @0x8368DA48 (opcode 0xC0, a float load); its image word is 0x00000000. */

#include <stdint.h>
#include "headers/hs_global_designator.h"
#include "headers/hs_global_external.h"
#include "headers/hs_global_datum.h"
#include "headers/data_array.h"
#include "headers/hs_type.h"
#include "headers/blam_data_globals.h"

extern int16_t hs_global_get_type(int16_t designator);

void hs_global_reconcile_read(int16_t global_designator)
{
    if ( !HS_GLOBAL_IS_EXTERNAL(global_designator) )
        return;

    hs_global_external *descriptor = hs_external_globals[HS_GLOBAL_DESIGNATOR_TO_INDEX(global_designator)];
    hs_global_datum *slot = DATA_ARRAY_ELEMENT(hs_global_data, hs_global_datum, HS_GLOBAL_DESIGNATOR_TO_INDEX(global_designator)); /* DEVIATION: masked slot index, per clrlslwi r10,r3,17,3 @0x8368D928 (was the raw flagged designator) */
    void *backing = descriptor->pointer; /* recovered: descriptor->address -> pointer (member at 0x08) */

    /* the 19 halfword cases share one backing load (lhz @0x8368DA60) and one tail (extsh/sth @0x8368DEF0) */
    int16_t word_value;

    switch ( hs_global_get_type(global_designator) )
    {
        case hs_type_boolean:                   /* case 0 @0x8368DA04 — lbz / stb */
            slot->value.boolean = backing ? *(unsigned char *)backing : hs_type_boolean_default;
            return;
        case hs_type_real:                      /* case 1 @0x8368DA2C — lfs / stfs */
            slot->value.real = backing ? *(float *)backing : hs_type_real_default;
            return;
        case hs_type_short_integer:             /* case 2 @0x8368DA54 — lhz / sth */
            word_value = backing ? *(int16_t *)backing : hs_type_short_integer_default; break;
        case hs_type_long_integer:              /* case 3 @0x8368DA74 — lwz / stw */
            slot->value.long_value = backing ? *(int *)backing : hs_type_long_integer_default;
            return;
        case hs_type_string:                    /* case 4 @0x8368DA9C — lwz / stw */
            slot->value.long_value = backing ? *(int *)backing : (int)hs_type_string_default;
            return;
        case hs_type_script:                    /* case 5 @0x8368DAC4 */
            word_value = backing ? *(int16_t *)backing : hs_type_script_default; break;
        case hs_type_trigger_volume:            /* case 6 @0x8368DADC */
            word_value = backing ? *(int16_t *)backing : hs_type_trigger_volume_default; break;
        case hs_type_cutscene_flag:             /* case 7 @0x8368DAF4 */
            word_value = backing ? *(int16_t *)backing : hs_type_cutscene_flag_default; break;
        case hs_type_cutscene_camera_point:     /* case 8 @0x8368DB0C */
            word_value = backing ? *(int16_t *)backing : hs_type_cutscene_camera_point_default; break;
        case hs_type_cutscene_title:            /* case 9 @0x8368DB24 */
            word_value = backing ? *(int16_t *)backing : hs_type_cutscene_title_default; break;
        case hs_type_cutscene_recording:        /* case 10 @0x8368DB3C */
            word_value = backing ? *(int16_t *)backing : hs_type_cutscene_recording_default; break;
        case hs_type_device_group:              /* case 11 @0x8368DB54 */
            word_value = backing ? *(int16_t *)backing : hs_type_device_group_default; break;
        case hs_type_ai:                        /* case 12 @0x8368DB6C — lwz / stw */
            slot->value.long_value = backing ? *(int *)backing : hs_type_ai_default;
            return;
        case hs_type_ai_command_list:           /* case 13 @0x8368DB94 */
            word_value = backing ? *(int16_t *)backing : hs_type_ai_command_list_default; break;
        case hs_type_starting_profile:          /* case 14 @0x8368DBAC */
            word_value = backing ? *(int16_t *)backing : hs_type_starting_profile_default; break;
        case hs_type_conversation:              /* case 15 @0x8368DBC4 */
            word_value = backing ? *(int16_t *)backing : hs_type_conversation_default; break;
        case hs_type_navpoint:                  /* case 16 @0x8368DBDC */
            word_value = backing ? *(int16_t *)backing : hs_type_navpoint_default; break;
        case hs_type_hud_message:               /* case 17 @0x8368DBF4 */
            word_value = backing ? *(int16_t *)backing : hs_type_hud_message_default; break;
        case hs_type_object_list:               /* case 18 @0x8368DC0C — lwz / stw */
            slot->value.long_value = backing ? *(int *)backing : hs_type_object_list_default;
            return;
        case hs_type_sound:                     /* case 19 @0x8368DC34 */
            slot->value.long_value = backing ? *(int *)backing : hs_type_sound_default;
            return;
        case hs_type_effect:                    /* case 20 @0x8368DC5C */
            slot->value.long_value = backing ? *(int *)backing : hs_type_effect_default;
            return;
        case hs_type_damage:                    /* case 21 @0x8368DC84 */
            slot->value.long_value = backing ? *(int *)backing : hs_type_damage_default;
            return;
        case hs_type_looping_sound:             /* case 22 @0x8368DCAC */
            slot->value.long_value = backing ? *(int *)backing : hs_type_looping_sound_default;
            return;
        case hs_type_animation_graph:           /* case 23 @0x8368DCD4 */
            slot->value.long_value = backing ? *(int *)backing : hs_type_animation_graph_default;
            return;
        case hs_type_actor_variant:             /* case 24 @0x8368DCFC */
            slot->value.long_value = backing ? *(int *)backing : hs_type_actor_variant_default;
            return;
        case hs_type_damage_effect:             /* case 25 @0x8368DD24 */
            slot->value.long_value = backing ? *(int *)backing : hs_type_damage_effect_default;
            return;
        case hs_type_object_definition:         /* case 26 @0x8368DD4C */
            slot->value.long_value = backing ? *(int *)backing : hs_type_object_definition_default;
            return;
        case hs_type_enum_game_difficulty:      /* case 27 @0x8368DD74 */
            word_value = backing ? *(int16_t *)backing : hs_type_enum_game_difficulty_default; break;
        case hs_type_enum_team:                 /* case 28 @0x8368DD8C */
            word_value = backing ? *(int16_t *)backing : hs_type_enum_team_default; break;
        case hs_type_enum_ai_default_state:     /* case 29 @0x8368DDA4 */
            word_value = backing ? *(int16_t *)backing : hs_type_enum_ai_default_state_default; break;
        case hs_type_enum_actor_type:           /* case 30 @0x8368DDBC */
            word_value = backing ? *(int16_t *)backing : hs_type_enum_actor_type_default; break;
        case hs_type_enum_hud_corner:           /* case 31 @0x8368DDD4 */
            word_value = backing ? *(int16_t *)backing : hs_type_enum_hud_corner_default; break;
        case hs_type_object:                    /* case 32 @0x8368DDEC — lwz / stw */
            slot->value.long_value = backing ? *(int *)backing : hs_type_object_default;
            return;
        case hs_type_unit:                      /* case 33 @0x8368DE14 */
            slot->value.long_value = backing ? *(int *)backing : hs_type_unit_default;
            return;
        case hs_type_vehicle:                   /* case 34 @0x8368DE3C */
            slot->value.long_value = backing ? *(int *)backing : hs_type_vehicle_default;
            return;
        case hs_type_weapon:                    /* case 35 @0x8368DE64 */
            slot->value.long_value = backing ? *(int *)backing : hs_type_weapon_default;
            return;
        case hs_type_device:                    /* case 36 @0x8368DE8C */
            slot->value.long_value = backing ? *(int *)backing : hs_type_device_default;
            return;
        case hs_type_scenery:                   /* case 37 @0x8368DEB4 */
            slot->value.long_value = backing ? *(int *)backing : hs_type_scenery_default;
            return;
        case hs_type_object_name:               /* case 38 @0x8368DEDC */
            word_value = backing ? *(int16_t *)backing : hs_type_object_name_default; break;
        default:
            return;
    }
    slot->value.short_value = word_value;   /* shared tail: extsh r11,r11 / sth r11, 4(r30) @0x8368DEF0 */
}
