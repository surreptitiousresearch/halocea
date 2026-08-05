/* object_cause_damage @0x836B35A0 — the master damage dispatcher. Given a damage_data event and a target
 * object, it: (1) randomises the damage amount within the definition's min/max and scales it by AI,
 * difficulty, friendly-fire and game-engine multipliers; (2) builds the list of "affected" objects (the
 * target plus its parent chain, or just the target for area damage), and — for a parent vehicle — splits
 * the impulse across seated players and recurses into each seat; (3) fires the local player screen/camera
 * effect for any affected object the local machine controls; (4) walks the affected list applying the
 * shield (object_damage_shield) and body (object_damage_body) portions, handling team/friendly-fire and
 * instakill gating, then records statistics and network notification (object_damage_aftermath,
 * damage_dealt_to_network) and deletes objects flagged for deletion.
 *
 * The decompiler models two adjacent stack buffers as a single damage_data 'v101': offsets 0..0x18 are the
 * working accumulators (definition_index = result flags, flags = shield damage dealt, owner_player_index =
 * body damage dealt, ...), offset 0x28 (epicenter) onward is the affected-object index array. They never
 * overlap, so this reconstruction keeps a single 'working' damage_data and aliases its epicenter as the
 * index array, matching the binary exactly.
 *
 * Deviation: verified the object_damage_shield (10-arg), object_damage_body (13-arg) and
 * object_damage_aftermath (8-arg) signatures against the call-site disasm (0x836B3F50, 0x836B4028,
 * 0x836B4108). The should_do_damage_effects byte (r25) the decompiler dropped is recovered from
 * 0x836B3C68-0x836B3CAC. player_effect_start_client is the deferred player-effect subsystem (extern). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/damage_resistance_flags.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/_object_definition.h"
#include "headers/vehicle_definition.h"
#include "headers/collision_model.h"
#include "headers/collision_node.h"
#include "headers/global_tag_instances.h"
#include "headers/damage_data.h"
#include "headers/damage_effect_definition.h"
#include "headers/damage_definition.h"
#include "headers/damage_definition_flags.h"
#include "headers/damage_resistance.h"
#include "headers/damage_material.h"
#include "headers/damage_dealt_network_data.h"
#include "headers/real_vector3d.h"
#include "headers/game_variant.h"
#include "headers/cheat_globals.h"
#include "headers/object_type.h"
#include "headers/damage_side_effect.h"
#include "headers/object_damage_flags.h"
#include "headers/damage_data_flags.h"
#include "headers/object_being_damaged_flags.h"
#include "headers/game_connection.h"
#include "headers/game_team.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
#include "headers/real_point3d.h"
extern void *datum_try_and_get(const data_array *data, int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags); /* was: int * — DB prototype returns void * */
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern int ai_adjust_damage(int actor_index, const damage_data *damage_data, float *damage);
extern uint8_t game_engine_running(void);
extern int get_player_index_from_object_or_parents(int object_index);
extern float game_engine_get_damage_multiplier(int attacker_index, int defender_index);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern float game_difficulty_get_value(int16_t value_type);
extern int16_t game_connection(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern void player_effect_start_server(int player_index, const damage_data *damage, const real_vector3d *direction, float scale, float total_damage);
extern void player_effect_start_client(int player_index, const damage_data *damage, const real_vector3d *direction, float scale, float total_damage);
extern game_variant *game_engine_get_variant(void);
extern uint8_t game_engine_has_teams(void);
extern int player_index_from_unit_index(int unit_index);
extern uint8_t unit_unsuspecting(int unit_index, const real_point3d *attacker);
extern void object_deplete_body(int object_index);
extern void object_delete(int object_index);
extern void object_damage_shield(int object_index, const damage_resistance *damage_resistance, const damage_material *damage_material, const damage_definition *damage_definition, const damage_data *damage_data, float *being_damaged_flags, float *shield_damage_reference, uint8_t should_do_actual_damage, uint8_t should_do_damage_effects, damage_dealt_network_data *replicated_damage_data);
extern void object_damage_body(int object_index, int16_t region_index, int16_t node_index, const real_vector3d *object_normal, const damage_resistance *damage_resistance, const damage_material *damage_material, const damage_definition *damage_definition, const damage_data *damage_data, float total_damage, unsigned int *damage_flags_accumulator, float *body_damage_reference, float *body_damage_multiplier_reference, uint8_t should_do_actual_damage);
extern void damage_dealt_to_network(int owner_player_index, damage_dealt_network_data data);
extern void object_damage_aftermath(int object_index, damage_data *damage_data, unsigned int being_damaged_flags, float shield_damage, float body_damage, float body_damage_multiplier, int16_t body_part, uint8_t should_do_actual_damage);

void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index,
                         int16_t region_index, int16_t material_index, const real_vector3d *object_normal)
{
    char directional_effect = 1;

    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    /* the local machine applies authoritative damage when it owns the object (network role 0 or 3) */
    int network_role = object->object.datum_role;
    uint8_t should_do_actual_damage = (!network_role || network_role == 3);

    if ( damage_data->owner_player_index != -1
      && !datum_try_and_get(player_data, damage_data->owner_player_index) )
        damage_data->owner_player_index = -1;

    char friendly_difficulty_applied = 0;
    char effect_not_suppressed = 1;

    damage_effect_definition *damage_tag_data = TAG_GET(damage_effect_definition, damage_data->definition_index);
    const damage_definition *damage_definition = &damage_tag_data->damage;

    /* base damage amount: lerp(min, random(lower,upper), scale) * event multiplier */
    /* recovered: *(float *)(damage_tag_data + 468/472) -> damage_definition->damage_lower_bound/upper_bound */
    float random_damage = real_seed_random_range(get_global_random_seed_address(),
                                                 damage_definition->damage_lower_bound,
                                                 damage_definition->damage_upper_bound);
    float total_damage = (((1.0f - damage_data->scale) * damage_definition->damage_minimum)
                          + (damage_data->scale * random_damage)) * damage_data->multiplier;

    /* AI difficulty/behaviour damage adjustment from the attacker's actor */
    if ( damage_data->owner_object_index != -1 )
    {
        unit_datum *attacker = object_try_and_get_and_verify_type(damage_data->owner_object_index, object_mask_unit);
        if ( attacker )
        {
            if ( attacker->unit.gunner_object_index != -1 )
                attacker = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                               attacker->unit.gunner_object_index)->datum;
            int actor_index = attacker->unit.swarm_actor_index == -1 ? attacker->unit.actor_index
                                                                     : attacker->unit.swarm_actor_index;
            if ( actor_index != -1 )
                ai_adjust_damage(actor_index, damage_data, &total_damage);
        }
    }

    if ( game_engine_running() )
    {
        int victim_player = get_player_index_from_object_or_parents(object_index);
        int attacker_player = get_player_index_from_object_or_parents(damage_data->owner_object_index);
        total_damage = game_engine_get_damage_multiplier(attacker_player, victim_player) * total_damage;
    }
    else
    {
        int16_t owner_team = (uint16_t)damage_data->owner_team_index;
        if ( owner_team != (int16_t)0xFFFF && game_team_is_enemy(owner_team, _game_team_player) )
        {
            total_damage = game_difficulty_get_value(0) * total_damage;
            friendly_difficulty_applied = 1;
        }
    }

    struct damage_data working;  /* struct tag: a param named damage_data shadows the typedef */
    int *affected_objects = (int *)&working.epicenter;  /* index array overlays epicenter onward */
    int16_t affected_count = 0;

    unsigned int flags = damage_data->flags;
    int is_area_damage = flags & (1u << _damage_area_of_effect_bit);
    if ( (flags & (1u << _damage_area_of_effect_bit)) != 0 || (flags & (1u << _damage_kill_instantly_bit)) != 0 )
    {
        affected_objects[0] = object_index;
        affected_count = 1;
    }
    else
    {
        int current = object_index;
        if ( object_index != -1 )
        {
            do
            {
                affected_objects[affected_count++] = current;
                current = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, current)
                              ->datum->object.parent_object_index;
            }
            while ( current != -1 );
        }
    }

    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);
    int object_collision_index = object_definition->collision_model.index;
    if ( object_collision_index != -1 )
        effect_not_suppressed =
            (~TAG_GET(collision_model, object_collision_index)->resistance.flags & (1u << _damage_resistance_parent_never_takes_body_damage_for_us_bit)) != 0;

    /* appended affected object (umbrella_shield_object_index) read as a float bit-pattern */
    float appended = *(float *)&object->object.umbrella_shield_object_index;
    if ( appended == appended )  /* not NaN (-1) */
        affected_objects[affected_count++] = *(int *)&appended;

    /* a damaged vehicle passes a fraction of the damage through to each seated player, then recurses */
    if ( !is_area_damage && object->object.type == object_type_vehicle )
    {
        damage_data->multiplier = (1.0f - damage_definition->vehicle_passthrough_penalty)
            * TAG_GET(vehicle_definition, object->definition_index)->unit.child_damage_fraction;
        if ( game_engine_running() )
        {
            int seated_player_count = 0;
            for ( int seat = object->object.first_child_object_index; seat != -1; )
            {
                unit_datum *occupant = (unit_datum *)
                    DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, seat)->datum;
                if ( !occupant->object.type && occupant->unit.player_index != -1 )
                    ++seated_player_count;
                seat = occupant->object.next_object_index;
            }
            if ( seated_player_count )
                damage_data->multiplier = damage_data->multiplier / (float)seated_player_count;
        }

        for ( int seat = object->object.first_child_object_index; seat != -1; )
        {
            unit_datum *occupant = (unit_datum *)
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, seat)->datum;
            if ( !occupant->object.type )
            {
                char damage_seat = 0;
                char seat_is_driver = 0;
                if ( occupant->unit.player_index == -1 )
                {
                    if ( seat == ((unit_datum *)object)->unit.driver_object_index )  /* empty driver seat */
                    {
                        damage_seat = 1;
                        seat_is_driver = 1;
                    }
                }
                else
                {
                    damage_seat = 1;
                }
                if ( damage_seat )
                {
                    if ( seat_is_driver )
                        damage_data->flags |= (1u << _damage_bypasses_shields_bit);
                    else
                        damage_data->flags &= ~(1u << _damage_bypasses_shields_bit);
                    object_cause_damage(damage_data, seat, -1, -1, -1, 0);
                    damage_data->flags &= ~(1u << _damage_bypasses_shields_bit);
                }
            }
            seat = occupant->object.next_object_index;
        }
        damage_data->multiplier = 1.0f;
    }

    if ( damage_definition->damage_lower_bound != 0.0f || damage_definition->damage_upper_bound != 0.0f )
        directional_effect = 0;

    /* fire the local player damage effect for any affected object the local machine controls */
    for ( int16_t i = 0; i < affected_count; ++i )
    {
        unit_datum *affected = object_try_and_get_and_verify_type(affected_objects[i], object_mask_unit);
        if ( !affected )
            continue;

        if ( affected->unit.player_index != -1 )  /* controlling player */
        {
            int16_t connection = game_connection();
            if ( (unsigned int)connection < NUMBER_OF_GAME_CONNECTIONS )
            {
                switch ( connection )
                {
                    case _game_connection_local:
                    default:
                        player_effect_start_client(affected->unit.player_index, damage_data, &damage_data->direction,
                                                   damage_data->scale, total_damage);
                        break;
                    case _game_connection_network_client:
                        if ( directional_effect == 1 )
                            player_effect_start_client(affected->unit.player_index, damage_data, &damage_data->direction,
                                                       damage_data->scale, total_damage);
                        break;
                    case _game_connection_network_server:
                        if ( directional_effect )
                            player_effect_start_client(affected->unit.player_index, damage_data, &damage_data->direction,
                                                       damage_data->scale, total_damage);
                        else
                            player_effect_start_server(affected->unit.player_index, damage_data, &damage_data->direction,
                                                       damage_data->scale, total_damage);
                        break;
                }
            }
        }
        else if ( cheat.reflexive_damage_effects )  /* unowned object: local cheat self-effect */
        {
            int16_t connection = game_connection();
            switch ( connection )
            {
                case _game_connection_local:
                case _game_connection_film_playback:
                    player_effect_start_client(local_player_get_player_index(0), damage_data,
                                               &damage_data->direction, damage_data->scale, total_damage);
                    break;
                case _game_connection_network_server:
                    player_effect_start_server(local_player_get_player_index(0), damage_data,
                                               &damage_data->direction, damage_data->scale, total_damage);
                    break;
            }
        }
    }

    if ( total_damage <= 0.0f )
        return;

    /* The 'working' damage_data is the shared accumulator the shield/body routines read and write through:
     *   working.definition_index  = result flags (deleted/depleted/shields-down/friendly bits)
     *   working.flags             = shield damage dealt (as float)
     *   working.owner_player_index= body damage dealt (as float)
     *   working.owner_team_index  = body damage multiplier (as float)
     * material_effect_recorded persists across the affected objects (set once per call). */
    char material_effect_recorded = 0;

    /* apply shield + body damage to each affected object, deepest (highest index) first */
    while ( affected_count-- > 0 )
    {
        *(float *)&working.flags = 0.0f;
        *(float *)&working.owner_player_index = 0.0f;
        *(float *)&working.owner_team_index = 0.0f;
        working.definition_index = 0;
        int16_t body_part = -1;

        int current_object_index = affected_objects[affected_count];
        object_datum *current_object =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, current_object_index)->datum;
        _object_definition *current_definition =
            TAG_GET(_object_definition, current_object->definition_index);
        int resistance_tag = current_definition->collision_model.index;

        if ( resistance_tag != -1 )
        {
            const collision_model *collision = TAG_GET(const collision_model, resistance_tag);
            const damage_resistance *resistance = &collision->resistance;
            int current_network_role = current_object->object.datum_role;
            char force_instakill = (damage_data->flags >> _damage_kill_instantly_bit) & 1;
            char victim_is_teammate = 0;
            char friendly_fire_damages_shield = 1;
            char friendly_fire_damages_body = 1;

            /* whether this machine should produce visible damage effects (recovered from disasm) */
            uint8_t should_do_damage_effects;
            if ( current_network_role == 3 || current_network_role == 0 )
            {
                should_do_damage_effects = 1;
            }
            else
            {
                should_do_damage_effects = 1;
                int role_owner = damage_data->owner_player_index;
                if ( role_owner != -1 )
                {
                    player_datum *role_player = datum_try_and_get(player_data, role_owner);
                    if ( role_player && (uint16_t)role_player->local_player_index != 0xFFFF )
                        should_do_damage_effects = 0;
                }
            }

            damage_dealt_network_data replicated_damage;
            replicated_damage.damaged_object_index = current_object_index;
            replicated_damage.shield_damage = 0.0f;
            replicated_damage.shields_depleted = 0;

            /* friendly-fire team handling: the branch is taken only when victim and attacker are teammates */
            int victim_player;
            player_datum *victim_player_data;
            uint8_t is_enemy = 1;
            if ( game_engine_running()
              && game_engine_has_teams()
              && (victim_player = player_index_from_unit_index(current_object_index)) != -1
              && victim_player != damage_data->owner_player_index
              && (victim_player_data = datum_try_and_get(player_data, victim_player)) != 0 )
            {
                /* Deviation fix: the old transcription read (victim + 8) — inside name[]; the DB
                 * decompile shows v81[8] on a dword pointer = offset 32 = player_datum.team_index. */
                is_enemy = game_team_is_enemy(victim_player_data->team_index,
                                              damage_data->owner_team_index);
                victim_is_teammate = (is_enemy == 0);
            }

            if ( !is_enemy )  /* same-team hit: apply the variant friendly-fire policy */
            {
                game_variant *variant = game_engine_get_variant();
                int friendly_fire = variant->universal_variant.friendly_fire;
                if ( friendly_fire == 2 )
                {
                    friendly_fire_damages_shield = 1;
                    friendly_fire_damages_body = 0;
                }
                else if ( friendly_fire == 3 && (damage_definition->flags & (1u << _damage_detonates_explosives_bit)) == 0 )
                {
                    /* full friendly fire: leave both shield and body damage enabled */
                }
                else
                {
                    friendly_fire_damages_shield = 0;
                    friendly_fire_damages_body = 0;
                }
            }

            /* body part struck, mapped from the region index via the collision node table */
            if ( region_index >= 0 && region_index < collision->nodes.count )
                body_part = ((collision_node *)collision->nodes.address)[region_index].runtime_damage_part;

            /* working.definition_index doubles as the object_being_damaged result-flags accumulator */
            if ( friendly_difficulty_applied )
                working.definition_index = (1u << _object_being_damaged_multiplied_by_difficulty_bit);

            int16_t owner_team = (uint16_t)damage_data->owner_team_index;
            if ( owner_team != (int16_t)0xFFFF
              && !game_team_is_enemy(current_object->object.owner_team_index, owner_team) )
                working.definition_index |= (1u << _object_being_damaged_by_friendly_bit);

            /* select the damage material for this region */
            const damage_material *material;
            if ( affected_count || material_index < 0 || material_index >= resistance->materials.count )
            {
                int indirect = resistance->indirect_damage_material_index;
                if ( indirect < 0 || indirect >= resistance->materials.count )
                    material = &default_damage_material;
                else
                    material = &((const damage_material *)resistance->materials.address)[indirect];
            }
            else
            {
                material = &((const damage_material *)resistance->materials.address)[material_index];
            }
            damage_data->material_type = material->type;

            if ( cheat.omnipotent && damage_data->owner_player_index != -1 )
                force_instakill = 1;
            if ( damage_definition->side_effect == _damage_side_effect_lethal_to_the_unsuspecting
              && unit_unsuspecting(current_object_index, &damage_data->epicenter)
              && (current_object->object.damage_flags & (1u << _object_cannot_take_damage_bit)) == 0 )
                force_instakill = 1;  /* assassination */

            if ( should_do_actual_damage == 1 && force_instakill
              && (current_object->object.damage_flags & (1u << _object_dead_bit)) == 0
              && (!victim_is_teammate || friendly_fire_damages_body) )
            {
                current_object->object.body_vitality = 0.0f;
                object_deplete_body(current_object_index);
                working.definition_index |= (1u << _object_being_damaged_killed_instantly_bit)
                                          | (1u << _object_being_damaged_body_depleted_bit);
            }

            if ( (damage_data->flags & (1u << _damage_bypasses_shields_bit)) == 0
              && (damage_definition->flags & (1u << _damage_skips_shields_bit)) == 0
              && current_object->object.maximum_shield_vitality > 0.0f
              && (!victim_is_teammate || friendly_fire_damages_shield)
              && (!affected_count || (resistance->flags & (1u << _damage_resistance_takes_shield_damage_for_children_bit)) != 0) )
            {
                object_damage_shield(current_object_index, resistance, material, damage_definition, &working,
                                     (float *)&working.flags, &total_damage, should_do_actual_damage,
                                     should_do_damage_effects, &replicated_damage);
            }

            if ( !affected_count || (effect_not_suppressed && (resistance->flags & (1u << _damage_resistance_takes_body_damage_for_children_bit)) != 0) )
            {
                if ( (damage_definition->flags & (1u << _damage_only_hurts_shields_bit)) == 0 )
                {
                    if ( ((resistance->flags & (1u << _damage_resistance_only_hurt_by_explosives_bit)) != 0 && (damage_definition->flags & (1u << _damage_detonates_explosives_bit)) == 0)
                      || (victim_is_teammate && !friendly_fire_damages_body) )
                        total_damage = 0.0f;

                    int16_t body_node = node_index;
                    int16_t body_region = region_index;
                    if ( affected_count )
                    {
                        body_node = -1;
                        body_region = -1;
                    }
                    object_damage_body(current_object_index, body_region, body_node,
                                       affected_count ? 0 : object_normal, resistance, material,
                                       damage_definition, damage_data, total_damage,
                                       &working.definition_index, (float *)&working.owner_player_index,
                                       (float *)&working.owner_team_index, should_do_actual_damage);
                    affected_count = 0;
                }
            }

            float shield_damage_dealt = *(float *)&working.flags;
            float body_damage_dealt = *(float *)&working.owner_player_index;

            /* record the material effect scale (shield vs body), once per call */
            if ( !material_effect_recorded
              && (shield_damage_dealt > 0.000099999997f || body_damage_dealt > 0.000099999997f) )
            {
                float material_effect_scale;
                if ( shield_damage_dealt <= body_damage_dealt )
                {
                    material_effect_scale = current_object->object.body_vitality;
                    if ( material_effect_scale >= 0.0f )
                    {
                        if ( material_effect_scale > 1.0f )
                            material_effect_scale = 1.0f;
                    }
                    else
                    {
                        material_effect_scale = 0.0f;
                    }
                }
                else
                {
                    damage_data->material_type = resistance->shield_material_type;
                    material_effect_scale = current_object->object.shield_vitality;
                }
                damage_data->material_effect_scale = material_effect_scale;
                material_effect_recorded = 1;
            }

            damage_dealt_to_network(damage_data->owner_player_index, replicated_damage);
            if ( shield_damage_dealt > 0.0f && !current_object->object.type )
                current_object->object.force_shield_update = 1;
        }

        /* shared aftermath / deletion for both the resolved and unresolved-resistance paths */
        object_damage_aftermath(current_object_index, damage_data, working.definition_index,
                                *(float *)&working.flags, *(float *)&working.owner_player_index,
                                *(float *)&working.owner_team_index, body_part, should_do_actual_damage);
        if ( (working.definition_index & (1u << _object_being_damaged_body_destroyed_bit)) != 0 )
            object_delete(current_object_index);
        if ( total_damage <= 0.0f )
            return;
    }
}
