/* effect_update @0x836E3758 — advance one live effect by dt. Tracks the effect to its host object (or
 * the object's ultimate parent), updates its BSP location/velocity, drives the scale/color functions,
 * culls it when no longer visible, and runs its event timeline: generating particles per frame and, when
 * an event elapses, randomly selecting the next event (or starting the next event's parts with randomized
 * per-part particle counts). Stops/deletes the effect at the end of a non-looping timeline.
 *
 * NOTE: the decompiler reported "local variable allocation failed"; the effect_real_random_range call and
 * its scale-flag arguments are heavily obscured by the optimizer. The identifiable per-part fields are
 * used; see the deviation comment at that call.
 *
 * Effect definition (v6): flags dword[0] (0x1 _effect_deleted_when_inactive, 0x4 _effect_must_be_deterministic), event count @52, event block
 * @56 (68-byte records: probability @4, duration random range @16/@20, part count @56, part block @60),
 * special end event index @4 / trigger event index @6. Effect part record (232 bytes): particle count
 * random range @108/@110 (words), scale-flag fields @224/@228. Parent object datum: object.flags
 * (_object_connected_to_map_bit), object.location, object.translational_velocity. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/effect_definition.h"
#include "headers/effect_event_definition.h"
#include "headers/effect_particles_definition.h"
#include "headers/_object_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/effect_datum.h"
#include "headers/effect_node_designator.h"
#include "headers/effect_location_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/effect_flags.h"
#include "headers/effect_definition_flags.h"
#include "headers/object_type.h"
#include "headers/object_datum.h"
#include "headers/object_flags.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern void *datum_get(data_array *data, int index);

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int object_get_ultimate_parent(int object_index);
extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern void effect_delete(int effect_index);
extern void effect_stop(int effect_index, uint8_t and_delete);
extern void effect_set_event(int effect_index, int16_t event_index);
extern void effect_generate_particles(effect_datum *effect);
extern void effect_generate_parts(effect_datum *effect);
extern uint8_t scenario_location_potentially_visible(const location *location);
extern uint8_t scenario_location_potentially_visible_local(const location *location);
extern uint32_t *get_global_random_seed_address(void);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float effect_real_random_range(uint32_t *seed, const effect_datum *effect, float lower_bound, float upper_bound, unsigned int scale_a_flags, unsigned int scale_b_flags, int16_t first_bit_index);
extern int16_t local_player_count(void);
extern effect_location_datum *effect_location_get_next_instance(const effect_datum *effect, int *location_datum_index, int16_t camera_mode);
extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern char *tag_get_name(int16_t tag_index);

void effect_update(int effect_index, float dt) /* was: double dt — DB prototype (f1 single-precision: fmr/fsubs/stfs) */
{
    /* recovered: effect_data->data + 252*(u16)effect_index -> datum_get (252 == effect datum_size incl. 2-byte salt header) */
    effect_datum *effect = datum_get(effect_data, effect_index);
    int object_index = effect->object_index;
    effect_definition *definition = TAG_GET(effect_definition, effect->definition_index);
    uint8_t visible;
    int16_t i;

    if ( object_index != -1 )
    {
        object_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_all);
        int ultimate_parent;
        object_datum *parent;

        if ( !object )
        {
            effect_delete(effect_index);
            return;
        }
        ultimate_parent = object_get_ultimate_parent(effect->object_index);
        parent = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ultimate_parent)->datum;
        /* DEVIATION: decompiler's raw dword view untangled to object_datum members; its (float)parent[26..28]
         * int->float conversions were wrong — the binary does plain word copies (lwz/stw @0x836E3944) */
        if ( (parent->object.flags & (1u << _object_connected_to_map_bit)) != 0 )
        {
            effect->location = parent->object.location;
            effect->velocity = parent->object.translational_velocity;
        }
        else
        {
            effect->location.cluster_index = -1;
        }

        if ( (effect->flags & (1u << _effect_loop_bit)) != 0 )
        {
            if ( object_get_function_value(effect->object_index, effect->scale_a_function_index, &effect->scale_a) )
            {
                uint16_t flags = effect->flags;
                if ( (flags & (1u << _effect_stopped_bit)) != 0 )
                {
                    if ( (flags & (1u << _effect_delete_on_stop_bit)) != 0 )
                    {
                        effect_delete(effect_index);
                    }
                    else
                    {
                        effect->flags = flags & ~(1u << _effect_stopped_bit);
                        effect_set_event(effect_index, 0);
                    }
                }
            }
            else if ( (definition->flags & (1u << _effect_deleted_when_inactive_bit)) != 0 )
            {
                /* looping effect whose driver went inactive: unlink it from the object's effect list */
                _object_definition *object_def = TAG_GET(_object_definition, object->definition_index);
                int marker_count = object_def->attachments.count;  /* def+320 = attachments block count */
                if ( marker_count > 0 )
                {
                    int slot = 0;
                    while ( object->object.attachment_indices[slot] != effect_index )
                    {
                        slot = (int16_t)(slot + 1);
                        if ( slot >= marker_count )
                        {
                            effect_delete(effect_index);
                            return;
                        }
                    }
                    object->object.attachment_indices[slot] = -1;
                }
                effect_delete(effect_index);
                return;
            }
            else
            {
                uint16_t flags = effect->flags;
                if ( (flags & (1u << _effect_stopped_bit)) == 0 && (flags & (1u << _effect_stopping_bit)) == 0 )
                    effect_stop(effect_index, 0);
            }

            object_get_function_value(effect->object_index, effect->scale_b_function_index, &effect->scale_b);
            if ( effect->change_color_index != -1 )
            {
                /* fix: outgoing_change_colors are floats; the decompiler's (float)*(uint*) was a
                 * numeric conversion of the bit pattern — read the float member directly. */
                real_rgb_color *color = &object->object.outgoing_change_colors[effect->change_color_index];
                effect->color.n[0] = color->n[0];
                effect->color.n[1] = color->n[1];
                effect->color.n[2] = color->n[2];
            }
        }
    }

    visible = 0;
    if ( (uint16_t)effect->location.cluster_index != 0xFFFF )
    {
        if ( (definition->flags & (1u << _effect_must_be_deterministic_bit)) != 0 )
            visible = scenario_location_potentially_visible(&effect->location);
        else
            visible = scenario_location_potentially_visible_local(&effect->location);
    }

    if ( !visible )
    {
        uint16_t flags = effect->flags;
        if ( (flags & (1u << _effect_invisible_bit)) != 0 )
            goto run_timeline;
        if ( (flags & (1u << _effect_loop_bit)) != 0 )
        {
            effect->flags = flags | (1u << _effect_invisible_bit);
            goto run_timeline;
        }
        effect_delete(effect_index);
        return;
    }

    {
        uint16_t flags = effect->flags;
        if ( (flags & (1u << _effect_invisible_bit)) != 0 )
            effect->flags = flags & ~(1u << _effect_invisible_bit);
    }

run_timeline:
    for ( i = 0; dt >= 0.0; i = (int16_t)(i + 1) )
    {
        uint16_t flags = effect->flags;
        char event_completed;
        float new_event_time;

        if ( (flags & (1u << _effect_stopped_bit)) != 0 || i >= 8 )
            break;

        if ( (effect->event_duration - effect->event_time) > dt )
        {
            new_event_time = (effect->event_time + dt);
            event_completed = 0;
            dt = -1.0;
        }
        else
        {
            event_completed = 1;
            dt = (dt - (effect->event_duration - effect->event_time));
        }
        effect->event_time = new_event_time;

        if ( (flags & (1u << _effect_in_duration_bit)) != 0 )
        {
            int next_event;

            if ( (flags & (1u << _effect_invisible_bit)) == 0 )
                effect_generate_particles(effect);
            if ( !event_completed )
                continue;

            if ( (effect->flags & (1u << _effect_loop_bit)) != 0
              && (uint16_t)effect->event_index == (uint16_t)definition->loop_stop_index
              && (uint16_t)definition->loop_start_index != 0xFFFF )
                next_event = definition->loop_start_index;
            else
                next_event = effect->event_index + 1;

            /* skip events whose probability roll fails */
            if ( (int16_t)next_event < definition->events.count )
            {
                do
                {
                    unsigned int *seed = (*TAG_GET(unsigned int, effect->definition_index) & (1u << _effect_must_be_deterministic_bit)) != 0
                        ? get_global_random_seed_address()
                        : get_global_local_random_seed_address();
                    if ( real_seed_random(seed) >= (double)((effect_event_definition *)definition->events.address)[(int16_t)next_event].skip_fraction )
                        break;
                    next_event = (int16_t)((int16_t)next_event + 1);
                }
                while ( (int16_t)next_event < definition->events.count );
            }

            if ( (int16_t)next_event >= definition->events.count )
            {
                uint16_t end_flags = effect->flags;
                if ( (end_flags & (1u << _effect_loop_bit)) == 0 )
                {
                    effect_delete(effect_index);
                    return;
                }
                effect->flags = end_flags | (1u << _effect_stopped_bit);
                return;
            }
            effect_set_event(effect_index, next_event);
        }
        else if ( event_completed )
        {
            effect_event_definition *event_block = (effect_event_definition *)definition->events.address;
            int16_t event_index = effect->event_index;
            effect_event_definition *event_record;
            unsigned int *seed;
            int part;

            effect->event_time = 0.0f;
            effect->flags = flags & ~(1u << _effect_in_duration_bit) | (1u << _effect_in_duration_bit);
            effect->last_event_fraction = -1.0f;
            event_record = &event_block[event_index];

            seed = (*TAG_GET(unsigned int, effect->definition_index) & (1u << _effect_must_be_deterministic_bit)) != 0
                ? get_global_random_seed_address()
                : get_global_local_random_seed_address();
            effect->event_duration = real_seed_random_range(seed, event_record->duration_lower_bound, event_record->duration_upper_bound);

            for ( part = 0; part < event_record->particles.count; part = (int16_t)(part + 1) )
            {
                effect_particles_definition *part_record =
                    &((effect_particles_definition *)event_record->particles.address)[part];
                float lower = (float)part_record->count_lower_bound;
                float upper = (float)part_record->count_upper_bound;
                int count;
                /* part record: scale_a flag mask @224, scale_b flag mask @228, bit index 5 (the two
                 * leading flag params of effect_real_random_range are unused). */
                count = (uint8_t)(int)effect_real_random_range(get_global_local_random_seed_address(),
                            effect, lower, upper, part_record->a_scales, part_record->b_scales, 5);
                effect->particles_counts[part] = count;
                if ( count > 6 )
                {
                    int players = local_player_count();
                    count = (int)((((float)count - 6.0f) / (float)players) + 6.0f);
                    effect->particles_counts[part] = count;
                }
            }

            if ( (effect->flags & (1u << _effect_invisible_bit)) == 0 )
                effect_generate_parts(effect);
        }
    }

    if ( hcex_debug_effect_names )
    {
        int location_index = effect->location_datum_indices[0];
        effect_location_datum *instance;

        tag_get_name(effect->definition_index);
        instance = effect_location_get_next_instance(effect, &location_index, 0);
        if ( instance )
        {
            uint16_t node_designator = (uint16_t)instance->node_designator;
            if ( node_designator != 0xFFFF )
            {
                const real_matrix4x3 *node_matrix;
                int16_t node_index;
                real_point3d transformed;

                if ( EFFECT_NODE_IS_FIRST_PERSON_WEAPON(node_designator) )
                {
                    node_matrix = first_person_weapon_get_node_matrix(effect->local_player_index, EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator));
                }
                else
                {
                    node_index = ((int16_t)node_designator == -1) ? -1 : EFFECT_NODE_DESIGNATOR_TO_INDEX(node_designator);
                    node_matrix = object_get_node_matrix(effect->object_index, node_index);
                }
                /* recovered: (char *)instance + 48 -> &instance->matrix.___u1.__s1.position (offset 0x30) */
                matrix4x3_transform_point(node_matrix, &instance->matrix.___u1.__s1.position, &transformed);
            }
        }
    }
}
