/* projectile_collision @0x8375B288 */
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#include <stdint.h>
#include "headers/ppc_intrinsics.h"
#include "headers/collision_result.h"
#include "headers/collision_surface_flags.h"
#include "headers/damage_data.h"
#include "headers/projectile_material_response_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_plane3d.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/networked_datum_role.h"
#include "headers/collision_result_type.h"
#include "headers/game_connection.h"
#include "headers/object_flags.h"
#include "headers/projectile_definition_flags.h"
#include "headers/projectile_datum_flags.h"
#include "headers/projectile_possible_response_flags.h"
#include "headers/projectile_material_response_type.h"
#include "headers/projectile_action.h"
#include "headers/damage_data_flags.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"
#include "headers/projectile_attach_message.h"

/* NOTE: the projectile object-datum tail (flags/material/detonation fields at
   raw byte offsets on `po`) and the projectile definition (`pdef`) are kept as
   raw offsets, per the object house style; the typed structs above cover the
   collision result, damage data and material-response definition. Blam engine
   symbols are extern-declared. */
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"
#include "headers/network_game_server.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_message_type.h"
typedef struct _field_properties_definition _field_properties_definition;

extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void object_cause_damage(damage_data *damage_data, int object_index, int16_t node_index, int16_t region_index, int16_t material_index, const real_vector3d *object_normal);
extern void breakable_surface_damage(int16_t breakable_surface_index, const damage_data *damage_data, int seed_surface_index);
extern float normalize3d(real_vector3d *v);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float real_seed_random(uint32_t *seed);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void component_vectors_from_direction3d(const real_vector3d *vector, const real_vector3d *direction, real_vector3d *parallel, real_vector3d *perpendicular);
extern real_vector3d *seed_random_vector_in_cone3d(uint32_t *seed, const real_vector3d *axis, float inner_cone_angle, float outer_cone_angle, real_vector3d *result);
extern real_vector3d *reflect_vector3d(const real_vector3d *incident, const real_vector3d *normal, real_vector3d *reflection);
extern float __fsqrts(float x);
extern void projectile_calculate_deceleration(int projectile_index);
extern void projectile_effect_new(int projectile_index, int definition_index, collision_result *collision, real_point3d *points, real_vector3d *vectors, float scale, float __noop);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern void object_attach_to_node(int parent_object_index, int child_object_index, int16_t parent_node_index);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern int16_t game_connection(void);

/* Response codes carried in `the_response` / the material-response definition
   are projectile_material_response_type (disappear/detonate/reflect/penetrate/attach). */
void projectile_collision(int projectile_index, collision_result *collision, real_point3d *new_position,
                          real_vector3d *new_velocity, float time_left)
{
    int16_t material_type = collision->material_type;
    float material_effect_scale = 0.0f;
    float damage_scale = 1.0f;
    projectile_datum *po = (projectile_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    projectile_definition *pdef = TAG_GET(projectile_definition, po->definition_index);
    real_vector3d unit_velocity;
    projectile_material_response_definition *response_def;
    real_plane3d *p_plane;
    float speed;
    float velocity_delta, incidence_angle;
    int16_t the_response;
    int effect_index;
    damage_data object_damage;
    damage_data surface_damage;
    real_vector3d effect_vectors[5]; /* projectile_effect_new's `vectors` arg; shares the surface_damage stack slot */
    real_point3d markers[5];
    unsigned int *seed;
    int i;

    (void)time_left;

    /* --- unit velocity + speed --- */
    unit_velocity = *new_velocity;
    speed = __fsqrts((new_velocity->n[2] * new_velocity->n[2])
                   + ((new_velocity->n[0] * new_velocity->n[0]) + (new_velocity->n[1] * new_velocity->n[1])));
    if (__fabs(speed) < 0.000099999997)
    {
        speed = 0.0f;
        unit_velocity = *global_up3d;
    }
    else
    {
        unit_velocity.n[0] = (1.0f / speed) * new_velocity->n[0];
        unit_velocity.n[1] = new_velocity->n[1] * (1.0f / speed);
        unit_velocity.n[2] = new_velocity->n[2] * (1.0f / speed);
    }

    /* --- speed-based damage scale over the penetration velocity range --- */
    if (pdef->projectile.final_velocity == pdef->projectile.initial_velocity)
    {
        damage_scale = 1.0f;
    }
    else
    {
        damage_scale = ((speed - pdef->projectile.final_velocity) / (pdef->projectile.initial_velocity - pdef->projectile.final_velocity));
        if (damage_scale < 0.0f) damage_scale = 0.0f;
        else if (damage_scale > 1.0f) damage_scale = 1.0f;
    }

    /* --- object hit: cause damage --- */
    if (collision->type == collision_result_object)
    {
        int damage_def = pdef->projectile.impact_damage.index;
        if (damage_def != -1)
        {
            damage_data_new(&object_damage, damage_def);
            object_damage.scale = damage_scale;
            object_damage.flags |= (1u << _damage_from_weapon_bit);
            object_damage.owner_player_index = po->object.owner_player_index;
            object_damage.owner_object_index = po->object.owner_object_index;
            object_damage.owner_team_index = po->object.owner_team_index;
            object_damage.epicenter.n[0] = collision->point.n[0];
            object_damage.epicenter.n[1] = collision->point.n[1];
            object_damage.epicenter.n[2] = collision->point.n[2];
            object_damage.origin.n[0] = collision->point.n[0];
            object_damage.origin.n[1] = collision->point.n[1];
            object_damage.origin.n[2] = collision->point.n[2];
            object_damage.direction.n[0] = new_velocity->n[0];
            object_damage.direction.n[1] = new_velocity->n[1];
            object_damage.direction.n[2] = new_velocity->n[2];
            normalize3d(&object_damage.direction);
            object_cause_damage(&object_damage, collision->object_index, collision->node_index,
                                collision->region_index, collision->material_index, (const real_vector3d *)&collision->plane);
            if ((uint16_t)object_damage.material_type != 0xFFFF)
                material_type = object_damage.material_type;
            material_effect_scale = object_damage.material_effect_scale;
        }
    }

    po->projectile.hit_material_type = material_type;

    /* --- select the material response definition --- */
    if (material_type < 0 || material_type >= pdef->projectile.material_responses.count)
        response_def = &default_projectile_material_response;
    else
        response_def = &((projectile_material_response_definition *)pdef->projectile.material_responses.address)[material_type];

    p_plane = &collision->plane;

    /* --- angle / velocity noise; incidence angle and velocity delta --- */
    {
        float along_plane = ((collision->plane.n.n[0] * new_velocity->n[0])
                                  + ((collision->plane.n.n[1] * new_velocity->n[1])
                                          + (new_velocity->n[2] * collision->plane.n.n[2])));
        seed = get_global_random_seed_address();
        velocity_delta = (real_seed_random_range(seed, -response_def->velocity_noise, 0.0f) - along_plane);
        seed = get_global_random_seed_address();
        incidence_angle = ((angle_between_vectors3d(new_velocity, &collision->plane.n) - 1.5707964f)
                                + real_seed_random_range(seed, -response_def->angle_noise, response_def->angle_noise));
    }

    /* --- choose the possible response if all gates pass, else the default --- */
    if (response_def->possible_response
        && (response_def->possible_response_maximum_angle == 0.0
            || (incidence_angle >= response_def->possible_response_minimum_angle
                && incidence_angle <= response_def->possible_response_maximum_angle))
        && (response_def->possible_response_maximum_velocity == 0.0
            || (velocity_delta >= response_def->possible_response_minimum_velocity
                && velocity_delta <= response_def->possible_response_maximum_velocity))
        && ((response_def->possible_response_flags & (1u << _projectile_possible_response_only_against_units_bit)) == 0
            || (collision->type == collision_result_object && object_try_and_get_and_verify_type(collision->object_index, object_mask_unit)))
        && (seed = get_global_random_seed_address(), real_seed_random(seed) >= response_def->possible_response_skip_fraction))
    {
        the_response = response_def->possible_response;
        effect_index = response_def->possible_response_effect.index;
    }
    else
    {
        the_response = response_def->default_response;
        effect_index = response_def->default_effect.index;
    }

    /* --- breakable-surface hit --- */
    if (collision->type == collision_result_structure && (collision->flags & (1u << _collision_surface_breakable_bit)) != 0)
    {
        damage_data_new(&surface_damage, pdef->projectile.impact_damage.index);
        surface_damage.flags |= (1u << _damage_from_weapon_bit);
        surface_damage.epicenter.n[0] = collision->point.n[0];
        surface_damage.epicenter.n[1] = collision->point.n[1];
        surface_damage.epicenter.n[2] = collision->point.n[2];
        surface_damage.origin.n[0] = collision->point.n[0];
        surface_damage.origin.n[1] = collision->point.n[1];
        surface_damage.origin.n[2] = collision->point.n[2];
        surface_damage.direction.n[0] = new_velocity->n[0];
        surface_damage.direction.n[1] = new_velocity->n[1];
        surface_damage.direction.n[2] = new_velocity->n[2];
        normalize3d(&surface_damage.direction);
        surface_damage.material_type = collision->material_type;
        if (surface_damage.material_type < 0 || surface_damage.material_type >= pdef->projectile.material_responses.count)
            surface_damage.material_response = &default_projectile_material_response;
        else
            surface_damage.material_response = &((const projectile_material_response_definition *)pdef->projectile.material_responses.address)[surface_damage.material_type];
        surface_damage.location = collision->location;
        breakable_surface_damage(collision->breakable_surface_index, &surface_damage, collision->surface_index);
    }

    new_position->n[0] = collision->point.n[0];
    new_position->n[1] = collision->point.n[1];
    new_position->n[2] = collision->point.n[2];

    /* --- penetrate --- */
    if (the_response == _projectile_response_penetrate)
    {
        if (collision->type)
        {
            if (collision->type == collision_result_object)
            {
                float keep = (1.0f - response_def->penetration_initial_friction);
                new_velocity->n[0] = keep * new_velocity->n[0];
                new_velocity->n[1] = new_velocity->n[1] * keep;
                new_velocity->n[2] = new_velocity->n[2] * keep;
                po->projectile.ignore_object_index = collision->object_index;
            }
            else if (pdef->projectile.detonation_maximum_time == 0.0)
            {
                the_response = _projectile_response_detonate; /* nothing to attach to -> detonate */
            }
            else
            {
                the_response = _projectile_response_attach;
                po->projectile.flags |= (1u << _projectile_collided_once_bit) | (1u << _projectile_stopped_after_collision_bit);
            }
        }
        else
        {
            /* environment: toggle the "wholly under media / in surface" flag, decelerate, nudge out of the plane */
            unsigned int flags = po->object.flags;
            po->object.flags = (flags & (1u << _object_wholly_under_media_bit))
                ? (flags & ~(1u << _object_wholly_under_media_bit))
                : (flags | (1u << _object_wholly_under_media_bit));
            projectile_calculate_deceleration(projectile_index);
            new_position->n[0] = -((p_plane->n.n[0] * 0.001f) - new_position->n[0]);
            new_position->n[1] = -((collision->plane.n.n[1] * 0.001f) - new_position->n[1]);
            new_position->n[2] = -((collision->plane.n.n[2] * 0.001f) - new_position->n[2]);
        }
    }

    /* --- ricochet / reflect --- */
    if (the_response == _projectile_response_reflect)
    {
        real_vector3d perp, parallel;
        component_vectors_from_direction3d(new_velocity, &collision->plane.n, &perp, &parallel);
        new_velocity->n[0] = ((1.0f - response_def->reflection_perpendicular_friction) * parallel.n[0])
                           - ((1.0f - response_def->reflection_parallel_friction) * perp.n[0]);
        new_velocity->n[1] = ((1.0f - response_def->reflection_perpendicular_friction) * parallel.n[1])
                           - ((1.0f - response_def->reflection_parallel_friction) * perp.n[1]);
        new_velocity->n[2] = ((1.0f - response_def->reflection_perpendicular_friction) * parallel.n[2])
                           - ((1.0f - response_def->reflection_parallel_friction) * perp.n[2]);
    }
    else if (the_response != _projectile_response_penetrate)
    {
        *new_velocity = *global_zero_vector3d;
    }

    /* --- apply angle + velocity noise to the outgoing velocity --- */
    if (response_def->angle_noise != 0.0)
    {
        seed = get_global_random_seed_address();
        seed_random_vector_in_cone3d(seed, new_velocity, 0.0f, response_def->angle_noise, new_velocity);
    }
    if (response_def->velocity_noise != 0.0)
    {
        float out_speed = normalize3d(new_velocity);
        if (out_speed != 0.0)
        {
            float scale;
            seed = get_global_random_seed_address();
            scale = (real_seed_random_range(seed, -response_def->velocity_noise, response_def->velocity_noise) + out_speed);
            new_velocity->n[0] = scale * new_velocity->n[0];
            new_velocity->n[1] = new_velocity->n[1] * scale;
            new_velocity->n[2] = new_velocity->n[2] * scale;
        }
    }

    /* --- detonation thresholds --- */
    {
        float speed_sq = ((new_velocity->n[2] * new_velocity->n[2])
                               + ((new_velocity->n[0] * new_velocity->n[0]) + (new_velocity->n[1] * new_velocity->n[1])));
        if (the_response != _projectile_response_attach && speed_sq < (pdef->projectile.detonation_minimum_velocity * pdef->projectile.detonation_minimum_velocity))
        {
            if (po->projectile.action < _projectile_action_detonate)
                po->projectile.action = _projectile_action_detonate;
        }
        if (speed_sq < 0.000099999997)
        {
            po->projectile.flags |= (1u << _projectile_stopped_after_collision_bit);
            if (collision->plane.n.n[2] > 0.30000001)
            {
                po->object.flags |= (1u << _object_at_rest_bit);
                if (pdef->projectile.detonation_maximum_time == 0.0)
                {
                    if (po->projectile.action < _projectile_action_detonate)
                        po->projectile.action = _projectile_action_detonate;
                }
            }
        }
    }

    /* --- effect scales --- */
    {
        float effect_scale;
        if (response_def->scale_effects_by == _projectile_material_response_scale_effects_by_angle)
        {
            effect_scale = (incidence_angle * 0.63661975f);
            if (effect_scale < 0.0f) effect_scale = 0.0f;
            else if (effect_scale > 1.0f) effect_scale = 1.0f;
        }
        else if (response_def->scale_effects_by == _projectile_material_response_scale_effects_by_damage)
        {
            effect_scale = damage_scale;
            if (effect_scale < 0.0f) effect_scale = 0.0f;
            else if (effect_scale > 1.0f) effect_scale = 1.0f;
        }
        else
        {
            effect_scale = 1.0f;
        }
        if (material_effect_scale < 0.0f) material_effect_scale = 0.0f;
        else if (material_effect_scale > 1.0f) material_effect_scale = 1.0f;

        /* --- build the effect payload for projectile_effect_new ---
         * DEVIATION: an earlier pass modelled this block as a repurposed `damage_data` and punned
         * every slot into it. The compiler does reuse the surface_damage stack slot here
         * (`addi r3, r1, var_1A0 # damage_data` -> damage_data_new @0x8375B614), but the payload
         * itself is projectile_effect_new's `vectors` argument — 5 consecutive real_vector3d at
         * var_1A0 (IDA labels it `# vectors` at both call sites, 0x8375BB10 / 0x8375BB54).
         * Store map @0x8375BA5C-0x8375BAB4 (var_1A0 = +0x00):
         *   +0x00 plane normal   +0x0C -unit_velocity   +0x18 unit_velocity
         *   +0x24 reflection (reflect_vector3d's out arg, `addi r5, r1, var_17C` = +0x24)
         *   +0x30 global_down3d (all three components)
         * The damage_data model put -velocity.z at +0x18 instead of +0x14, left +0x1C unwritten,
         * and aimed reflect_vector3d at +0x28 — which also clobbered the down vector's x. --- */
        effect_vectors[0] = collision->plane.n;
        effect_vectors[1].n[0] = unit_velocity.n[0] * -1.0f;
        effect_vectors[1].n[1] = unit_velocity.n[1] * -1.0f;
        effect_vectors[1].n[2] = unit_velocity.n[2] * -1.0f;
        effect_vectors[2] = unit_velocity;
        effect_vectors[4] = *global_down3d;
        reflect_vector3d(&unit_velocity, &collision->plane.n, &effect_vectors[3]);

        for (i = 0; i < 5; ++i)
        {
            markers[i].n[0] = collision->point.n[0];
            markers[i].n[1] = collision->point.n[1];
            markers[i].n[2] = collision->point.n[2];
        }

        if (velocity_delta > 0.0083333338)
            projectile_effect_new(projectile_index, effect_index, collision, markers, effect_vectors, effect_scale, material_effect_scale);

        {
            unsigned int po_flags = po->projectile.flags;
            if ((po_flags & (1u << _projectile_counting_down_bit)) == 0
                && ((po_flags & (1u << _projectile_stopped_after_collision_bit)) != 0 || the_response == _projectile_response_attach))
                projectile_effect_new(projectile_index, pdef->projectile.detonation_timer_started.index, collision, markers, effect_vectors, effect_scale, material_effect_scale);
        }
    }

    /* --- finalize by response --- */
    if (the_response != _projectile_response_disappear)
    {
        if (the_response != _projectile_response_detonate)
        {
            if (the_response != _projectile_response_attach)
                return;

            /* attach */
            if (collision->type == collision_result_object && game_connection() && game_connection() != _game_connection_film_playback)
            {
                object_datum *target = object_try_and_get_and_verify_type(collision->object_index, object_mask_all);
                if (target && !target->object.type && (target->object.damage_flags & (1u << _object_dead_bit)) == (1u << _object_dead_bit))
                    return;
            }
            if (collision->type == collision_result_object)
            {
                if (po->object.datum_role == _networked_datum_puppet)
                    return;
                if ((pdef->projectile.flags & (1u << _projectile_super_combining_explosion_bit)) != 0)
                {
                    /* detach any sibling projectiles already stuck in this object */
                    unsigned char count = 0;
                    int child = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, collision->object_index)->datum)->object.first_child_object_index;
                    while (child != -1)
                    {
                        projectile_datum *child_obj = (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child)->datum;
                        if (child_obj->definition_index == po->definition_index && (child_obj->projectile.flags & (1u << _projectile_already_super_exploded_bit)) == 0)
                        {
                            child_obj->projectile.arming_time = 0.0f;
                            child_obj->projectile.detonation_timer = 0.0f;
                            ++count;
                        }
                        if (count >= 6)
                        {
                            po->projectile.flags |= (1u << _projectile_will_super_explode_bit);
                            break;
                        }
                        child = child_obj->object.next_object_index;
                    }
                }
            }

            po->projectile.flags |= (1u << _projectile_attached_bit);
            po->object.flags |= (1u << _object_at_rest_bit);
            po->object.translational_velocity = *global_zero_vector3d;
            po->object.angular_velocity = *global_zero_vector3d;
            object_translate(projectile_index, new_position, &collision->location);
            if (collision->type == collision_result_object)
                object_attach_to_node(collision->object_index, projectile_index, collision->node_index);

            /* schedule detonation timer */
            {
                int def_flags = pdef->projectile.flags;
                float ticks;
                if ((def_flags & (1u << _projectile_detonation_max_time_if_attached_bit)) != 0)
                {
                    ticks = (pdef->projectile.detonation_maximum_time * 30.0f);
                }
                else if ((def_flags & (1u << _projectile_random_detonation_time_when_attached_bit)) == 0)
                {
                    goto attach_network; /* no timed detonation */
                }
                else
                {
                    seed = get_global_random_seed_address();
                    ticks = (real_seed_random_range(seed, pdef->projectile.detonation_minimum_time, pdef->projectile.detonation_maximum_time) * 30.0f);
                }
                if (ticks >= 1.0f)
                    po->projectile.detonation_timer_delta = 1.0f / ticks;
            }

attach_network:
            if (collision->type == collision_result_object && po->object.datum_role == _networked_datum_master)
            {
                int object_index = collision->object_index;
                object_datum *target_datum = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
                if (target_datum->object.datum_role == _networked_datum_master)
                {
                    /* DEVIATION: the decompiler's real_vector3d word-punned payload is the DB-typed
                       projectile_attach_network_data message (decoded by projectile_attach_from_network).
                       The previous `*((uint16_t *)&msg.n[2] + 1)` spelling put the node index at +0xA;
                       disasm 0x8375BE1C stores it at +0x8 (parent_node_index) — defect fixed. */
                    projectile_attach_message msg;
                    int size;
                    void *server;
                    msg.projectile_index = field_translated_index_translate_index(&field_properties_object_index_definition, projectile_index);
                    msg.parent_index = field_translated_index_translate_index(&field_properties_object_index_definition, object_index);
                    msg.parent_node_index = collision->node_index;
                    size = message_delta_processor_encode_stateless(_message_projectile_attach, 0, &msg, g_message_encode_buffer, 32760);
                    server = global_network_game_server_get();
                    network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
                                                                            g_message_encode_buffer, size, 1u, 0, 0, 3);
                    po->object.flags |= (1u << _object_force_baseline_update_bit);
                }
            }
            return;
        }

        /* detonate */
        if (po->projectile.action < _projectile_action_detonate)
            po->projectile.action = _projectile_action_detonate;
    }
    else
    {
        /* none/stop */
        if (po->projectile.action < _projectile_action_disappear)
            po->projectile.action = _projectile_action_disappear;
    }
}
