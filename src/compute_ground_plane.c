/* compute_ground_plane @0x837BE138 — resolve the ground contact plane under one physics mass point. Seeds the
 * mass point's stored plane with the "depths of hell" fallback plane and a depth estimate, then does a collision
 * sphere query around the mass point centre (radius = the mass-point definition's sphere radius) and a point
 * test against the returned features. On a hit it records the contact plane, contact depth, and the struck
 * surface's material sound index — from the structure BSP collision materials for world surfaces, or via the
 * struck object's collision-model materials for object surfaces — sets/clears the mass point's "in contact"
 * flag (bit 2) unless the contact is a non-walkable feature (flags&8) or the object isn't shield-bearing, and
 * depletes the struck object's shield.
 *
 * DEVIATION: the DB 3-arg prototype is authoritative; the decompiler's RtlCheckStack12 stack-probe misrender
 * fabricated the a4..a47 phantom-parameter pile, which is really this frame's stack scratch — a
 * collision_feature_list (the sphere query result) and a collision_plane (the point-test result). Register
 * disasm resolved the calls: collision_get_features_in_sphere's three float args burn GPR shadows so its
 * ignore_object_index is `object_index` and its features out-param is the local; collision_features_test_point
 * fills the local collision_plane. mass_point fields are named (ground_plane @+0x60, ground_material_type
 * @+0x70, ground_depth @+0x74, flags @+0, position @+4); mass_point_definition is opaque (radius @+104). */

#include <stdint.h>
#include "headers/mass_point_datum.h"
#include "headers/mass_point_definition.h"
#include "headers/damage_material.h"
#include "headers/collision_model.h"
#include "headers/collision_feature_list.h"
#include "headers/collision_plane.h"
#include "headers/collision_result_type.h"
#include "headers/real_plane3d.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/structure_bsp.h"
#include "headers/structure_collision_material.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"

struct mass_point_definition;


extern uint8_t collision_get_features_in_sphere(unsigned int flags, const real_point3d *center, float radius, float height, float width, int ignore_object_index, collision_feature_list *features);
extern uint8_t collision_features_test_point(const collision_feature_list *features, const real_point3d *point, collision_plane *collision);
extern void object_deplete_shield(int object_index);


extern int16_t get_material_type(int object_index, int16_t material_index);
void compute_ground_plane(int object_index, mass_point_datum *mass_point,
        const struct mass_point_definition *mass_point_definition)
{
    const real_point3d *center = &mass_point->position;
    float radius = mass_point_definition->radius;

    /* seed with the fallback plane + an estimated penetration depth */
    real_plane3d *ground_plane = &mass_point->ground_plane;
    *ground_plane = depths_of_hell;
    mass_point->ground_material_type = -1;
    mass_point->ground_depth = radius
        - (center->n[0] * ground_plane->n.n[0]
         + center->n[2] * ground_plane->n.n[2]
         + center->n[1] * ground_plane->n.n[1]
         - ground_plane->d);

    collision_feature_list features;
    if ( !collision_get_features_in_sphere(_collision_test_for_bipeds_dead_flags, center, radius, 0.0f, radius, object_index, &features) )
        return;

    collision_plane collision;
    if ( !collision_features_test_point(&features, center, &collision) )
        return;

    mass_point->ground_depth = collision.t;
    *ground_plane = collision.plane;

    int reference_object = collision.object_index;
    int16_t material_index = collision.material_index;

    /* DEVIATION: donor get_material_type@0x837BE090's full body (all three branches) was inlined verbatim here with reference_object/material_index as the runtime args; collapsed to a call. */
    int16_t material_sound = get_material_type(reference_object, material_index);
    mass_point->ground_material_type = material_sound;

    unsigned char contact_flags = collision.flags;
    /* collision_result.flags bits are indexed by collision_result_type: bit 3 = hit-an-object */
    if ( (contact_flags & (1u << collision_result_object)) == 0
      && (reference_object == -1
       || ((1 << DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, reference_object)->type) & object_mask_scenery) != 0) )
        mass_point->flags &= ~4u;
    else
        mass_point->flags |= 4u;

    if ( reference_object != -1 )
        object_deplete_shield(reference_object);
}
