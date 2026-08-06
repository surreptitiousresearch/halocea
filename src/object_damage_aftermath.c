/* object_damage_aftermath @0x836B1AA0 — post-damage bookkeeping for a single damage event: applies the
 * physics impulse (acceleration) the damage imparts to the object, fires HCEX achievement/player events
 * on a kill, and records multiplayer scoring/statistics. Runs after the shield and body damage have
 * already been applied.
 *
 * Three stages:
 *   1. Acceleration — scale the (normalised, slightly-upward-biased) damage direction by the damage
 *      definition's acceleration and the object definition's response, then dispatch to the type-specific
 *      accelerate routine (biped/vehicle/item/projectile). Item acceleration is replicated to all
 *      machines via an item_accelerate message-delta.
 *   2. HCEX events — when this aftermath is the authoritative pass (a11), fire "player_damage" for the
 *      damaged player and, on a kill (being_damaged_flags&1), map the killed object's / killer's tag name
 *      to an achievement event (kill_grunt, kill_floodcombat, kill_floodinfection, kill_banshee,
 *      kill_wraith_with_scorpion).
 *   3. Scoring — record damage/kill statistics or, when scoring is disabled / a suicide, notify the game
 *      engine of the kill. Finally, hand bipeds/vehicles to unit_damage_aftermath.
 *
 * Deviation: verified the real 8-arg signature against the prologue (0x836B1AA0) and caller (0x836B40E8).
 * The decompiler's body_part/should_do_actual_damage/a9/a10/a11 are float-skip-reserved GPR phantoms; the
 * authoritative kill-pass gate is a11 and the body-part value stored to hcex_last_killed_in_body_part is
 * a10. The inlined byte-compare loops are strcmp; rewritten as strcmp() with this note. The network
 * message subsystem and game_engine scoring/statistics are the Blam extern boundary. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/damage_data.h"
#include "headers/real_vector3d.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/item_datum.h"
#include "headers/item_flags.h"
#include "headers/player_datum.h"
#include "headers/_object_definition.h"
#include "headers/damage_definition.h"
#include "headers/damage_effect_definition.h"
#include "headers/damage_definition_flags.h"
#include "headers/damage_data_flags.h"
#include "headers/object_being_damaged_flags.h"
#include "headers/networked_datum_role.h"
#include "headers/object_type.h"
#include "headers/unit_flags.h"

typedef struct network_game_server network_game_server;
struct field_properties_definition;

#define NETWORK_MESSAGE_TYPE_MESSAGE_DELTA 9
#define MESSAGE_ITEM_ACCELERATE            3
#define MESSAGE_ENCODE_BUFFER_BITS         0x7FF8

#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"
typedef struct _field_properties_definition _field_properties_definition;
extern int strcmp(const char *string1, const char *string2);

extern float normalize3d(real_vector3d *v);
extern uint8_t unit_driver_seat_is_filled(int unit_index);
extern void vehicle_accelerate(int vehicle_index, const real_vector3d *acceleration);
extern void biped_accelerate(int biped_index, real_vector3d *acceleration);
extern void item_accelerate(int item_index, const real_vector3d *acceleration, uint8_t detonates_explosives);
extern void projectile_accelerate(int projectile_index, const real_vector3d *acceleration);
extern int player_index_from_unit_index(int unit_index);
extern void  hcex_fire_plr_event(const char *event_name, int player_identifier); /* DEVIATION: slot 1 is int, not short -- def src/hcex/hcex_fire_plr_event.cpp + prologue stw r4 @0x823E4730 */
extern void *datum_try_and_get(const data_array *data, int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t game_engine_can_score(void);
extern void game_engine_player_killed(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire);
extern void game_statistics_record_damage(int object_index, float damage, int owner_player_index, int owner_object_index, int16_t owner_team_index);
extern void game_statistics_record_kill(int object_index, int owner_player_index, int owner_object_index, int16_t owner_team_index);
extern void unit_damage_aftermath(int unit_index, damage_data *damage_data, char being_damaged_flags, float shield_damage, float body_damage, float body_damage_multiplier, int16_t body_part, uint8_t unit_is_alive_gate);

/* Deviation: shield_damage/body_damage/body_damage_multiplier are single-precision floats widened to
 * double by Hex-Rays. Disasm passes them via fadds-fed FPRs with no frsp (836B20F0) and the caller loads
 * body_damage_multiplier with lfs (836B4114) — param-width precedent (f-reg feeding fadds directly) = float.
 * unit_damage_aftermath (unattested) is still declared double; it likewise takes float (width note only). */
void object_damage_aftermath(int object_index, damage_data *damage_data, unsigned int being_damaged_flags,
                             float shield_damage, float body_damage, float body_damage_multiplier,
                             int16_t body_part, uint8_t should_do_actual_damage)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    /* +452 = damage_effect_definition.damage (jpt! tag body) */
    damage_definition *damage_def =
        &TAG_GET(damage_effect_definition, damage_data->definition_index)->damage;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);
    float definition_acceleration_response = object_definition->acceleration_scale;

    if ( definition_acceleration_response > 0.000099999997f )
    {
        real_vector3d accel_direction;
        accel_direction.n[2] = damage_data->direction.n[2] + 0.44999999f;  /* slight upward bias */
        accel_direction.n[0] = damage_data->direction.n[0];
        accel_direction.n[1] = damage_data->direction.n[1];
        normalize3d(&accel_direction);

        float acceleration_magnitude =
            (definition_acceleration_response * damage_def->instantaneous_acceleration) * SECONDS_PER_TICK;
        real_vector3d acceleration_vector;
        acceleration_vector.n[0] = accel_direction.n[0] * acceleration_magnitude;
        acceleration_vector.n[1] = accel_direction.n[1] * acceleration_magnitude;
        acceleration_vector.n[2] = acceleration_magnitude * accel_direction.n[2];

        unsigned int object_type = (uint16_t)object->object.type;
        if ( object_type <= object_type_projectile )
        {
            if ( !object->object.type || object_type == object_type_vehicle )  /* biped or vehicle */
            {
                if ( damage_def->instantaneous_acceleration > 0.000099999997f
                  && (((unit_datum *)object)->unit.flags & (1u << _unit_impervious_bit)) == 0 )
                {
                    if ( (int16_t)object_type )
                    {
                        if ( (int16_t)object_type == object_type_vehicle )  /* vehicle */
                        {
                            if ( (damage_def->flags & (1u << _damage_detonates_explosives_bit)) != 0 )  /* double impulse */
                            {
                                acceleration_vector.n[0] = (accel_direction.n[0] * acceleration_magnitude) * 2.0f;
                                acceleration_vector.n[1] = (accel_direction.n[1] * acceleration_magnitude) * 2.0f;
                                acceleration_vector.n[2] = (acceleration_magnitude * accel_direction.n[2]) * 2.0f;
                            }
                            if ( object->object.datum_role != _networked_datum_puppet || unit_driver_seat_is_filled(object_index) == 1 )
                                vehicle_accelerate(object_index, &acceleration_vector);
                        }
                    }
                    else
                    {
                        biped_accelerate(object_index, &acceleration_vector);
                    }
                }
            }
            /* DEVIATION: unsigned range test (object_type - 3 < 2) unfolded into the
             * equipment/garbage members it encodes */
            else if ( object_type == object_type_weapon || object_type == object_type_equipment
                   || object_type == object_type_garbage )
            {
                char accelerate_item = 0;
                float ax = accel_direction.n[0] * acceleration_magnitude;
                float ay = accel_direction.n[1] * acceleration_magnitude;
                float az = acceleration_magnitude * accel_direction.n[2];
                if ( (ay * ay + (ax * ax + az * az)) >= 0.000099999997f
                  || (((item_datum *)object)->item.flags & (1u << _item_on_structure_bit)) == 0 )
                    accelerate_item = 1;

                if ( !object->object.datum_role && accelerate_item == 1 )
                {
                    /* replicate item acceleration to all machines */
                    int translated_index = field_translated_index_translate_index_no_default(
                        &field_properties_object_index_definition, object_index);
                    struct {
                        int   object_index;          /* 0 */
                        float magnitude;             /* 1 */
                        real_vector3d direction;     /* 2..4 */
                    } item_accelerate_message;
                    item_accelerate_message.object_index = translated_index;
                    item_accelerate_message.magnitude = acceleration_magnitude;
                    item_accelerate_message.direction = accel_direction;

                    int size_in_bits = message_delta_processor_encode_stateless(
                        MESSAGE_ITEM_ACCELERATE, 0, &item_accelerate_message,
                        g_message_encode_buffer, MESSAGE_ENCODE_BUFFER_BITS);
                    network_game_server *server = global_network_game_server_get();
                    network_game_server_send_message_to_all_loaded_machines(
                        server, NETWORK_MESSAGE_TYPE_MESSAGE_DELTA, g_message_encode_buffer, size_in_bits,
                        0, 0, 0, MESSAGE_ITEM_ACCELERATE);
                }

                int network_role = object->object.datum_role;
                if ( !network_role || network_role == 3 || !accelerate_item )
                {
                    uint8_t large_item = 0;
                    if ( damage_data->scale > 0.5 && (damage_def->flags & (1u << _damage_detonates_explosives_bit)) != 0 )
                        large_item = 1;
                    item_accelerate(object_index, &acceleration_vector, large_item);
                }
            }
            else
            {
                projectile_accelerate(object_index, &acceleration_vector);
            }
        }
    }

    if ( should_do_actual_damage == 1 )
    {
        data_array *players = player_data;
        int16_t damaged_player_identifier;
        if ( player_index_from_unit_index(object_index) == -1 )
        {
            damaged_player_identifier = -1;
        }
        else
        {
            int damaged_player_index = player_index_from_unit_index(object_index);
            damaged_player_identifier =
                DATA_ARRAY_ELEMENT(player_data, player_datum, damaged_player_index)->local_player_index;
        }
        if ( damaged_player_identifier != -1 )
        {
            hcex_fire_plr_event("player_damage", damaged_player_identifier);
            players = player_data;
        }

        if ( (being_damaged_flags & (1u << _object_being_damaged_body_depleted_bit)) != 0 && damage_data->owner_player_index != -1 )
        {
            const char *killed_tag_name = object_definition->model.name;
            hcex_last_killed_in_body_part = body_part;
            if ( killed_tag_name )
            {
                int owner_player_index = damage_data->owner_player_index;
                int16_t owner_player_identifier =
                    DATA_ARRAY_ELEMENT(players, player_datum, owner_player_index)->local_player_index;
                const char *event_name = "kill_wraith_with_scorpion";  /* default when all checks fall through */
                /* Deviation: the following byte-compare chains are inlined strcmp. */
                if ( strcmp(killed_tag_name, "characters\\grunt\\grunt") == 0 )
                {
                    event_name = "kill_grunt";
                }
                else if ( strcmp(killed_tag_name,
                                 "characters\\floodcombat_human\\floodcombat_human") == 0
                       || strcmp(killed_tag_name,
                                 "characters\\floodcombat elite\\floodcombat elite") == 0 )
                {
                    event_name = "kill_floodcombat";
                }
                else if ( strcmp(killed_tag_name,
                                 "characters\\flood_infection\\flood_infection") == 0
                       || strcmp(killed_tag_name,
                                 "characters\\flood_infection\\flood_infection nopop") == 0 )
                {
                    event_name = "kill_floodinfection";
                }
                else if ( strcmp(killed_tag_name, "vehicles\\banshee\\banshee") == 0 )
                {
                    event_name = "kill_banshee";
                }
                else if ( strcmp(killed_tag_name, "vehicles\\wraith\\wraith") == 0 )
                {
                    /* killed in a wraith: award only if the killer was driving a scorpion */
                    player_datum *killer_player = datum_try_and_get(players, owner_player_index);
                    unit_datum *killer_object = object_try_and_get_and_verify_type(killer_player->unit_index, object_mask_unit);
                    if ( killer_object->object.parent_object_index != -1 )
                    {
                        object_datum *killer_vehicle = DATA_ARRAY_ELEMENT(object_header_data,
                            object_header_datum, killer_object->object.parent_object_index)->datum;
                        const char *killer_vehicle_name =
                            TAG_GET(_object_definition, killer_vehicle->definition_index)
                                ->model.name;
                        if ( killer_vehicle_name
                          && strcmp(killer_vehicle_name, "vehicles\\scorpion\\scorpion") == 0 )
                            hcex_fire_plr_event("kill_wraith_with_scorpion", owner_player_identifier);
                    }
                    goto scoring;
                }
                else
                {
                    goto scoring;  /* unrecognised tag: no achievement event */
                }
                hcex_fire_plr_event(event_name, owner_player_identifier);
            }
        }

scoring:
        /* scoring is only reached on the authoritative pass (should_do_actual_damage) */
        if ( !game_engine_can_score() || (damage_data->flags & (1u << _damage_no_statistics_bit)) != 0 )
        {
            if ( game_engine_can_score() )
            {
                int killed_player_index = player_index_from_unit_index(object_index);
                game_engine_player_killed(killed_player_index, object_index, killed_player_index, 1u);
            }
        }
        else
        {
            game_statistics_record_damage(object_index, shield_damage + body_damage, 0,
                                          damage_data->owner_player_index, damage_data->owner_object_index);
            if ( (being_damaged_flags & (1u << _object_being_damaged_body_depleted_bit)) != 0 )
                game_statistics_record_kill(object_index, damage_data->owner_player_index,
                                            damage_data->owner_object_index, damage_data->owner_team_index);
        }
    }

    int object_type_mask = (1 << object->object.type) & object_mask_unit;
    if ( object_type_mask )  /* biped or vehicle */
        unit_damage_aftermath(object_index, damage_data, being_damaged_flags, shield_damage, body_damage,
                              body_damage_multiplier, body_part, object_type_mask);
}
