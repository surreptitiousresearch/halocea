/* effect_generate_part @0x836E18F0 — spawn one part of a live effect at a resolved marker location. The
 * part's tag group (runtime_tag_reference_base_class_tag) selects the spawn path:
 *   'pctl' (particle system) — random launch velocity + effect velocity, particle_system_new_unattached.
 *   'snd!' (sound)           — object_impulse_sound_new (attached to the effect's object) or
 *                              unattached_impulse_sound_new (built from world point/forward).
 *   'obje' (object)          — object_placement_data_new, random translational + angular velocity, object_new.
 *   'deca' (decal)           — random velocity + radius modifier, decal_new.
 *   'jpt!' (damage / jolt)   — damage_data_new + area_of_effect_cause_damage.
 *   'ligh' (light)           — light_new_unattached, only when rasterizer_effects_level > 0.
 *
 * Special case: the "burning_flame/effects/burning" scenery effect suppresses its decal ('deca') part.
 *
 * NOTE: the 'snd!' unattached path and the 'jpt!' path reuse the object_placement_data stack buffer as a
 * sound_location / damage_data respectively, hence the type-punned writes below (mirroring the database).
 * NOTE: in the attached 'snd!' path the decompiler lost the is_local_player computation (it is derived from
 * the controlling player datum looked up via datum_try_and_get); reconstructed as 0 with the lookup kept. */

#include <stdint.h>
#include "headers/effect_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_datum.h"
#include "headers/effect_part_definition.h"
#include "headers/effect_location_datum.h"
#include "headers/object_placement_data.h"
#include "headers/damage_data.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_argb_color.h"
#include "headers/location.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

struct data_array;
struct sound_location;
struct decal_editor_geometry;


#include "headers/sound_location.h"
#include "headers/decal_editor_geometry.h"
extern char *tag_get_name(int16_t tag_index);
extern uint32_t *get_global_random_seed_address(void);
extern uint32_t *get_global_local_random_seed_address(void);
extern void effect_random_translational_velocity(uint32_t *seed, const effect_datum *effect, const real_vector3d *forward, real_vector3d *direction, real_vector3d *velocity, float lower_bound, float upper_bound, float cone_angle, unsigned int a_scale_flags, unsigned int b_scale_flags);
extern float effect_real_random_range(uint32_t *seed, const effect_datum *effect, float lower_bound, float upper_bound, unsigned int scale_a_flags, unsigned int scale_b_flags, int16_t first_bit_index);
extern real_vector3d *seed_random_direction3d(uint32_t *seed, real_vector3d *direction);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

extern int particle_system_new_unattached(int definition_index, const real_point3d *position, const real_vector3d *velocity, const real_argb_color *color, float scale);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);
extern int unattached_impulse_sound_new(int definition_index, const sound_location *location, float scale, uint8_t is_player);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void *datum_try_and_get(const data_array *data, int index);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);
extern void decal_new(int definition_index, const real_point3d *origin, const real_vector3d *velocity, float radius_modifier, uint8_t permanent, int16_t forced_sequence_index, decal_editor_geometry *editor_geometry);
extern void damage_data_new(damage_data *damage_data, int definition_index);
extern void area_of_effect_cause_damage(damage_data *damage_data, int unlucky_object_index);
extern int light_new_unattached(int definition_index, int object_index, int16_t node_index, real_point3d *position, real_vector3d *forward, float scale);

void effect_generate_part(const effect_datum *effect, const effect_part_definition *part_definition,
        effect_location_datum *location_instance, const real_point3d *world_point, const real_vector3d *world_forward,
        const real_vector3d *world_up, float scale)
{
    unsigned int base_class_tag;
    real_vector3d direction;
    real_vector3d velocity;
    real_argb_color color;
    object_placement_data placement;

    /* Suppress the decal part of the burning_flame scenery effect. */
    {
        const char *name = tag_get_name(effect->definition_index);
        if ( name )
        {
            const char *match = "scenery\\emitters\\burning_flame\\effects\\burning";
            int delta = 0;
            while ( 1 )
            {
                if ( !*name )
                    break;
                delta = (unsigned __int8)*name - (unsigned __int8)*match;
                if ( delta )
                    break;
                ++name;
                ++match;
            }
            if ( !delta && part_definition->runtime_tag_reference_base_class_tag == 0x64656361u /* 'deca' */ )
                return;
        }
    }

    base_class_tag = part_definition->runtime_tag_reference_base_class_tag;

    if ( base_class_tag > 0x6F626A65 /* 'obje' */ )
    {
        if ( base_class_tag == 0x7063746Cu )   /* 'pctl' — particle system */
        {
            unsigned int *seed = get_global_local_random_seed_address();
            color.n[0] = 1.0f;
            color.n[1] = effect->color.n[0];
            color.n[2] = effect->color.n[1];
            color.n[3] = effect->color.n[2];
            effect_random_translational_velocity(seed, effect, world_forward, &direction, &velocity,
                    part_definition->velocity_lower_bound, part_definition->velocity_upper_bound,
                    part_definition->velocity_cone_angle, part_definition->a_scales, part_definition->b_scales);
            velocity.n[0] = effect->velocity.n[0] + velocity.n[0];
            velocity.n[1] = effect->velocity.n[1] + velocity.n[1];
            velocity.n[2] = effect->velocity.n[2] + velocity.n[2];
            particle_system_new_unattached(part_definition->reference.index, world_point, &velocity, &color, scale);
        }
        else if ( base_class_tag == 0x736E6421u )   /* 'snd!' — sound */
        {
            if ( effect->object_index == -1 )
            {
                /* Build an unattached sound_location over the placement buffer (type-punned, per the DB). */
                location effect_location = effect->location;
                placement.owner_object_definition_index = *(const int *)&world_forward->n[1];
                placement.owner_player_index            = *(const int *)&world_point->n[2];
                *(float *)&placement.owner_team_index   = world_forward->n[2];
                placement.definition_index              = *(const int *)&world_point->n[0];
                placement.flags                         = *(const int *)&world_point->n[1];
                placement.owner_object_index            = *(const int *)&world_forward->n[0];
                placement.position.n[0] = global_zero_vector3d->n[0];
                placement.position.n[1] = global_zero_vector3d->n[1];
                placement.position.n[2] = global_zero_vector3d->n[2];
                effect_location.leaf_index = *(const int *)&placement.position.n[0];
                *(location *)&placement.height = effect_location;
                unattached_impulse_sound_new(part_definition->reference.index,
                        (const struct sound_location *)&placement, scale,
                        (unsigned __int8)placement.owner_object_index);
            }
            else
            {
                unsigned __int8 is_local_player = 0;   /* see header note: original derives this from the player datum */
                __int16 node_index;
                unit_datum *object = object_try_and_get_and_verify_type(effect->owner_object_index, object_mask_unit);
                if ( object )
                    datum_try_and_get(player_data, object->unit.player_index);  /* unit+536 */
                node_index = location_instance->node_designator;
                if ( node_index != -1 )
                    node_index &= 0x7FFF;
                object_impulse_sound_new(effect->object_index, part_definition->reference.index, node_index,
                        (const real_point3d *)&location_instance->matrix.___u1.n[3],
                        (const real_vector3d *)&location_instance->matrix.___u1.n[0], scale, is_local_player);
            }
        }
        return;
    }

    switch ( base_class_tag )
    {
        case 0x6F626A65u:   /* 'obje' — object */
        {
            unsigned int *seed;
            float angular_speed;
            object_placement_data_new(&placement, part_definition->reference.index, effect->owner_object_index);
            placement.position.n[0] = world_point->n[0];
            placement.position.n[1] = world_point->n[1];
            placement.position.n[2] = world_point->n[2];
            placement.forward.n[0] = world_forward->n[0];
            placement.forward.n[1] = world_forward->n[1];
            placement.forward.n[2] = world_forward->n[2];
            placement.up.n[0] = world_up->n[0];
            placement.up.n[1] = world_up->n[1];
            placement.up.n[2] = world_up->n[2];
            effect_random_translational_velocity(get_global_random_seed_address(), effect, world_forward,
                    &direction, &placement.translational_velocity,
                    part_definition->velocity_lower_bound, part_definition->velocity_upper_bound,
                    part_definition->velocity_cone_angle, part_definition->a_scales, part_definition->b_scales);
            placement.translational_velocity.n[0] = effect->velocity.n[0] + placement.translational_velocity.n[0];
            placement.translational_velocity.n[1] = effect->velocity.n[1] + placement.translational_velocity.n[1];
            placement.translational_velocity.n[2] = effect->velocity.n[2] + placement.translational_velocity.n[2];
            seed = get_global_random_seed_address();
            angular_speed = effect_real_random_range(seed, effect,
                    part_definition->angular_velocity_lower_bound, part_definition->angular_velocity_upper_bound,
                    part_definition->a_scales, 0, 0);
            if ( angular_speed == 0.0 )
            {
                placement.angular_velocity.n[0] = global_zero_vector3d->n[0];
                placement.angular_velocity.n[1] = global_zero_vector3d->n[1];
                placement.angular_velocity.n[2] = global_zero_vector3d->n[2];
            }
            else
            {
                seed_random_direction3d(seed, &placement.angular_velocity);
                placement.angular_velocity.n[0] = placement.angular_velocity.n[0] * angular_speed;
                placement.angular_velocity.n[1] = placement.angular_velocity.n[1] * angular_speed;
                placement.angular_velocity.n[2] = placement.angular_velocity.n[2] * angular_speed;
            }
            object_new(&placement);
            break;
        }

        case 0x64656361u:   /* 'deca' — decal */
        {
            float radius_modifier;
            effect_random_translational_velocity(get_global_local_random_seed_address(), effect, world_forward,
                    &direction, &velocity, part_definition->velocity_lower_bound,
                    part_definition->velocity_upper_bound, part_definition->velocity_cone_angle,
                    part_definition->a_scales, part_definition->b_scales);
            radius_modifier = real_seed_random_range(get_global_local_random_seed_address(),
                    part_definition->radius_modifier_lower_bound, part_definition->radius_modifier_upper_bound);
            decal_new(part_definition->reference.index, world_point, &velocity, radius_modifier, 0, 0,
                    (struct decal_editor_geometry *)0xFFFFFFFF);
            break;
        }

        case 0x6A707421u:   /* 'jpt!' — damage / jolt */
        {
            location effect_location = effect->location;
            object_datum *owner = object_try_and_get_and_verify_type(effect->owner_object_index, object_mask_all);
            damage_data_new((damage_data *)&placement, part_definition->reference.index);
            if ( owner )
            {
                placement.owner_player_index = owner->object.owner_player_index;
                placement.owner_object_index = effect->owner_object_index;
                /* high word carries the owner object's team index */
                placement.owner_object_definition_index =
                        (placement.owner_object_definition_index & 0xFFFF) | ((unsigned __int16)owner->object.owner_team_index << 16);
            }
            placement.up.n[0] = scale;
            /* damage_data overlay: epicenter/origin built from the marker location + world point/forward */
            *(int *)&placement.translational_velocity.n[0] = effect_location.leaf_index;
            placement.translational_velocity.n[1] = world_point->n[1];
            placement.translational_velocity.n[2] = world_point->n[2];
            *(int *)&placement.position.n[1] = effect_location.leaf_index;
            placement.position.n[2] = world_point->n[1];
            placement.height = world_point->n[2];
            placement.forward.n[0] = world_forward->n[0];
            placement.forward.n[1] = world_forward->n[1];
            placement.forward.n[2] = world_forward->n[2];
            *(location *)&placement.owner_team_index = effect_location;
            area_of_effect_cause_damage((damage_data *)&placement, -1);
            break;
        }

        default:
            if ( base_class_tag == 0x6C696768u /* 'ligh' */
                    && rasterizer_debug_options.rasterizer_effects_level > 0 )
            {
                __int16 node_index = location_instance->node_designator;
                if ( node_index != -1 )
                    node_index &= 0x7FFF;
                light_new_unattached(part_definition->reference.index, effect->object_index, node_index,
                        (real_point3d *)&location_instance->matrix.___u1.n[3],
                        (real_vector3d *)&location_instance->matrix.___u1.n[0], scale);
            }
            break;
    }
}
