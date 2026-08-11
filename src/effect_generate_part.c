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
 * NOTE: the compiler reused one stack slot for the object_placement_data / sound_location / damage_data
 * locals; reconstructed as three properly typed locals (disasm-verified field maps at each call site).
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
extern char *tag_get_name(int tag_index);
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
                delta = (uint8_t)*name - (uint8_t)*match;
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
                /* DEVIATION: the decompiler rendered this as puns over the reused placement stack slot;
                 * the binary (0x836E1D84-0x836E1DEC) builds a plain sound_location: position/forward from
                 * the marker, zero velocity, and the effect's location copied whole (no leaf zeroing). */
                sound_location snd;
                snd.position = *world_point;
                snd.forward = *world_forward;
                snd.translational_velocity = *global_zero_vector3d;
                snd.game_location = effect->location;
                /* DEVIATION: is_player is 0 (`li r6, 0` @0x836E1D90). Float-slot skip — r5 is `scale`'s shadow, so IDA's 5-arg prototype mislabels the r5 scratch load @0x836E1DAC (it only feeds snd.forward.n[0]); the callee reads r6 (`mr r30, r6` @0x83713AA8), never r5. */
                unattached_impulse_sound_new(part_definition->reference.index, &snd, scale, 0);
            }
            else
            {
                uint8_t is_local_player = 0;   /* see header note: original derives this from the player datum */
                int16_t node_index;
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
            /* DEVIATION: same float-shadow mis-map. effect_real_random_range takes two floats, so its
             * trailing three args are r7/r8/r9: 0x836E1C20-0x836E1C2C sets r7 = [part_definition+0x60]
             * (a_scales), r8 = [part_definition+0x64] (b_scales) and r9 = 3. A prior revision dropped
             * b_scales to 0 and passed first_bit_index = 0, so the angular-velocity scaling read the
             * wrong flag pair and scale_b never applied. */
            angular_speed = effect_real_random_range(seed, effect,
                    part_definition->angular_velocity_lower_bound, part_definition->angular_velocity_upper_bound,
                    part_definition->a_scales, part_definition->b_scales, 3);
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
            /* DEVIATION: decal_new's `float radius_modifier` consumes the r6 GPR slot as well as f1, so its
             * trailing args are r7/r8/r9, not r6/r7/r8 (decal_new never reads r6 — see src/decal_new.c).
             * The binary's argument block at 0x836E1B18-0x836E1B24 is `li r9,0` / `li r8,-1` / `li r7,0`,
             * i.e. permanent=0, forced_sequence_index=-1, editor_geometry=NULL. A prior revision read the
             * decompiler's one-register-left mapping and passed forced_sequence_index=0 with 0xFFFFFFFF as
             * the editor_geometry pointer; decal_new_from_collision dereferences that pointer whenever it
             * is non-null. Same defect and same cause as unit_place.c / damage_dealt_from_network.c. */
            decal_new(part_definition->reference.index, world_point, &velocity, radius_modifier, 0, -1, nullptr);
            break;
        }

        case 0x6A707421u:   /* 'jpt!' — damage / jolt */
        {
            /* DEVIATION: the decompiler rendered this as puns over the reused placement stack slot; the
             * binary (0x836E1A14-0x836E1AAC) fills a plain damage_data: owner ids/team, the effect's
             * location copied whole, origin+epicenter = marker point, direction = marker forward, scale. */
            damage_data damage;
            object_datum *owner = object_try_and_get_and_verify_type(effect->owner_object_index, object_mask_all);
            damage_data_new(&damage, part_definition->reference.index);
            if ( owner )
            {
                damage.owner_player_index = owner->object.owner_player_index;
                damage.owner_object_index = effect->owner_object_index;
                damage.owner_team_index = owner->object.owner_team_index;
            }
            damage.scale = scale;
            damage.location = effect->location;
            damage.origin = *world_point;
            damage.epicenter = *world_point;
            damage.direction = *world_forward;
            area_of_effect_cause_damage(&damage, -1);
            break;
        }

        default:
            if ( base_class_tag == 0x6C696768u /* 'ligh' */
                    && rasterizer_debug_options.rasterizer_effects_level > 0 )
            {
                int16_t node_index = location_instance->node_designator;
                if ( node_index != -1 )
                    node_index &= 0x7FFF;
                light_new_unattached(part_definition->reference.index, effect->object_index, node_index,
                        (real_point3d *)&location_instance->matrix.___u1.n[3],
                        (real_vector3d *)&location_instance->matrix.___u1.n[0], scale);
            }
            break;
    }
}
