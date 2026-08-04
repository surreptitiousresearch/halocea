/* particle_system_new_attached @0x8373BE60 — attach a particle system to an object marker. Gated on the
 * global render_particle_systems_enabled. Allocates a particle-system datum (344-byte stride), records
 * definition/object/attachment and the per-attachment function index (object definition dword 81 =
 * attachments block, 72-byte records: function word @+48, change-color node word @+52, marker name
 * @+16). When a change-color node is present it samples that node's color (object_data node block at
 * +110/+111/+112) with full alpha, else uses ARGB white. Resolves the marker position, samples object
 * velocity (scaled to per-second by *30), seeds the tint to RGB white, evaluates the density function
 * into the flags, and initializes the system. Returns the index, or -1 (deleting the datum on
 * initialize failure). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/particle_system_datum.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_attachment_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/real_argb_color.h"
#include "headers/particle_system_flags.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern void datum_delete(data_array *data, int index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern uint8_t particle_system_initialize(int new_system_index);

int particle_system_new_attached(int definition_index, int object_index, int16_t attachment_index)
{
    if ( !render_particle_systems_enabled )
        return -1;

    int particle_system_index = datum_new(particle_systems);
    if ( particle_system_index == -1 )
        return particle_system_index;

    particle_system_datum *ps =
        DATA_ARRAY_ELEMENT(particle_systems, particle_system_datum, particle_system_index);
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int *object_data = (int *)object;
    object_attachment_definition *attachments = (object_attachment_definition *)
        TAG_GET(_object_definition, object->definition_index)->attachments.address;

    ps->definition_index = definition_index;
    object_attachment_definition *attachment_ref = &attachments[attachment_index];
    ps->object_index = object_index;
    ps->attachment_index = attachment_index;
    ps->function_index = attachment_ref->primary_scale_function_reference - 1;

    __int16 change_color_node = attachment_ref->change_color_reference;
    if ( change_color_node )
    {
        int *node_color = &object_data[3 * change_color_node];
        *(int *)&ps->color.n[1] = node_color[110];
        *(int *)&ps->color.n[2] = node_color[111];
        ps->color.n[0] = 1.0;
        *(int *)&ps->color.n[3] = node_color[112];
    }
    else
    {
        ps->color = *global_real_argb_white;
    }

    object_marker marker;
    object_get_marker_by_name(object_index, attachment_ref->marker_name, &marker, 1);
    ps->position.n[0] = marker.matrix.n[3][0];
    ps->position.n[1] = marker.matrix.n[3][1];
    ps->position.n[2] = marker.matrix.n[3][2];

    object_get_velocities(object_index, &ps->velocity, 0);
    ps->velocity.n[2] = ps->velocity.n[2] * 30.0f;
    ps->velocity.n[0] = ps->velocity.n[0] * 30.0f;
    ps->velocity.n[1] = ps->velocity.n[1] * 30.0f;
    ps->lighting = *global_real_rgb_white;

    unsigned char function_value = object_get_function_value(object_index, ps->function_index, &ps->scale);
    unsigned int flags = ps->flags | (1u << _particle_system_active_bit);
    if ( !function_value )
        flags = ps->flags & ~(1u << _particle_system_active_bit);
    ps->flags = flags;

    if ( particle_system_initialize(particle_system_index) )
        return particle_system_index;

    datum_delete(particle_systems, particle_system_index);
    return -1;
}
