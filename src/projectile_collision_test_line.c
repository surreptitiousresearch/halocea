/* projectile_collision_test_line @0x83759E38 — test a projectile's movement from its current origin to
 * new_position for collision. First runs a straight-line (zero-thickness) collision_test_vector along the path,
 * ignoring the projectile's own object; on a hit it reports immediately. If the projectile definition has a
 * non-trivial radius (definition +416), it additionally runs two parallel "edge" tests offset to either side of
 * the path by that radius, so a thin projectile still catches surfaces its centerline would slip past. The
 * lateral offset direction is up x path, normalized (falling back to global_left3d when the path is parallel to
 * up). Returns 1 on any hit, else 0.
 *
 * The object is fetched through the standard OBJECT_DATA idiom; object position is at +92 (real_point3d), the
 * ignore-object index at +564, and the object's definition tag index at +0. The two offset tests use the vector
 * form (start + delta) for the +radius side and the line form (start, end) for the -radius side, mirroring the
 * binary exactly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/collision_result.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"


extern float normalize3d(real_vector3d *v);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern uint8_t collision_test_line(unsigned int flags, const real_point3d *point0, const real_point3d *point1, int ignore_object_index, collision_result *collision);


uint8_t projectile_collision_test_line(int projectile_index, const real_point3d *new_position, collision_result *collision_result)
{
    projectile_datum *object = ((projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum);
    const real_point3d *object_position = &object->object.position;
    int ignore_object_index = object->projectile.ignore_object_index;
    const projectile_definition *definition =
            TAG_GET(projectile_definition, object->definition_index);

    real_vector3d path;
    path.n[0] = new_position->n[0] - object_position->n[0];
    path.n[1] = new_position->n[1] - object_position->n[1];
    path.n[2] = new_position->n[2] - object_position->n[2];
    if ( collision_test_vector(_collision_test_for_projectiles_flags, object_position, &path, ignore_object_index, collision_result) )
        return 1;

    float radius = definition->projectile.collision_radius;
    if ( radius < 0.000099999997f )
        return 0;

    /* lateral offset direction = up x path */
    real_vector3d offset;
    offset.n[0] = global_up3d->n[1] * (new_position->n[2] - object_position->n[2])
                - global_up3d->n[2] * (new_position->n[1] - object_position->n[1]);
    offset.n[1] = global_up3d->n[2] * (new_position->n[0] - object_position->n[0])
                - global_up3d->n[0] * (new_position->n[2] - object_position->n[2]);
    offset.n[2] = global_up3d->n[0] * (new_position->n[1] - object_position->n[1])
                - global_up3d->n[1] * (new_position->n[0] - object_position->n[0]);
    if ( normalize3d(&offset) == 0.0f )
        offset = *global_left3d;

    real_point3d right_start;
    right_start.n[0] = offset.n[0] * radius + object_position->n[0];
    right_start.n[1] = offset.n[1] * radius + object_position->n[1];
    right_start.n[2] = offset.n[2] * radius + object_position->n[2];

    real_point3d left_start;
    left_start.n[0] = -radius * offset.n[0] + object_position->n[0];
    left_start.n[1] = offset.n[1] * -radius + object_position->n[1];
    left_start.n[2] = offset.n[2] * -radius + object_position->n[2];

    real_vector3d right_path;
    right_path.n[0] = (radius * offset.n[0] + new_position->n[0]) - right_start.n[0];
    right_path.n[1] = (offset.n[1] * radius + new_position->n[1]) - right_start.n[1];
    right_path.n[2] = (offset.n[2] * radius + new_position->n[2]) - right_start.n[2];

    real_point3d left_end;
    left_end.n[0] = -radius * offset.n[0] + new_position->n[0];
    left_end.n[1] = offset.n[1] * -radius + new_position->n[1];
    left_end.n[2] = offset.n[2] * -radius + new_position->n[2];

    if ( collision_test_vector(_collision_test_for_projectiles_fat_flags, &right_start, &right_path, ignore_object_index, collision_result) )
        return 1;
    if ( collision_test_line(_collision_test_for_projectiles_fat_flags, &left_start, &left_end, object->projectile.ignore_object_index, collision_result) )
        return 1;
    return 0;
}
