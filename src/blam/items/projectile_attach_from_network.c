/* projectile_attach_from_network @0x837598C0 — client handler for the "projectile attach" message delta: attach
 * a networked projectile to a node on a parent object. Follows the standard net-delta template — discard the body
 * if this is a decoding-information (mode) pass, otherwise decode the stateless payload and translate both object
 * indices to local. With a valid projectile (type flag 0x20) and parent, and a projectile definition that allows
 * it (definition flag 0x8), it walks the parent's existing children and, for up to six children that share the
 * projectile's definition and are not already flagged (child flag 0x40), zeroes two of their timing fields;
 * hitting six marks the projectile (flag 0x80). It then flags the projectile as attached, zeroes its linear and
 * angular velocity, attaches it to the parent node, and seeds its per-frame rate field from the definition — a
 * fixed rate for a continuous (flag 0x4) projectile, or a random-range rate for a variable (flag 0x20) one.
 *
 * Object lookups use the standard OBJECT_DATA idiom; object->definition is *(int*)TAG_INSTANCE(*(int*)object).
 * Raw object/definition field offsets are reproduced from the decompiler (object: flags @16/@556, velocity @104,
 * angular velocity @140, rate @580, first child @280, next sibling @276; definition: flags @380, rate @448,
 * rate-min @444). */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/field_properties_definition.h"
#include "headers/projectile_attach_message.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/object_flags.h"
#include "headers/projectile_definition_flags.h"
#include "headers/projectile_datum_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void object_attach_to_node(int parent_object_index, int child_object_index, int16_t parent_node_index);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);


void projectile_attach_from_network(message_delta_processor_header *header)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    projectile_attach_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    int projectile_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
            message.projectile_index);
    int parent_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
            message.parent_index);

    projectile_datum *projectile = object_try_and_get_and_verify_type(projectile_index, object_mask_projectile);
    if ( !projectile )
        return;
    if ( !object_try_and_get_and_verify_type(parent_index, object_mask_all) )
        return;

    projectile_definition *definition = TAG_GET(projectile_definition, projectile->definition_index);
    if ( (definition->projectile.flags & (1u << _projectile_super_combining_explosion_bit)) != 0 )
    {
        int matched = 0;
        int child_index = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum)->object.first_child_object_index;
        while ( child_index != -1 )
        {
            projectile_datum *child = ((projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, child_index)->datum);
            if ( child->definition_index == projectile->definition_index
              && (child->projectile.flags & (1u << _projectile_already_super_exploded_bit)) == 0 )
            {
                child->projectile.arming_time = 0.0f;
                child->projectile.detonation_timer = 0.0f;
                ++matched;
            }
            if ( matched >= 6 )
            {
                projectile->projectile.flags |= (1u << _projectile_will_super_explode_bit);
                break;
            }
            child_index = child->object.next_object_index;
        }
    }

    projectile->projectile.flags |= (1u << _projectile_attached_bit);
    projectile->object.flags |= (1u << _object_at_rest_bit);
    projectile->object.translational_velocity = *global_zero_vector3d;
    projectile->object.angular_velocity = *global_zero_vector3d;

    object_attach_to_node(parent_index, projectile_index, message.parent_node_index);

    unsigned int definition_flags = definition->projectile.flags;
    if ( (definition_flags & (1u << _projectile_detonation_max_time_if_attached_bit)) != 0 )
    {
        if ( definition->projectile.detonation_maximum_time * 30.0f >= 1.0f )
            projectile->projectile.detonation_timer_delta = 1.0f / (definition->projectile.detonation_maximum_time * 30.0f);
    }
    else if ( (definition_flags & (1u << _projectile_random_detonation_time_when_attached_bit)) != 0 )
    {
        float rate_max = definition->projectile.detonation_maximum_time;
        float rate_min = definition->projectile.detonation_minimum_time;
        float rate = real_seed_random_range(get_global_random_seed_address(), rate_min, rate_max);
        if ( rate * 30.0f >= 1.0f )
            projectile->projectile.detonation_timer_delta = 1.0f / (rate * 30.0f);
    }
}
