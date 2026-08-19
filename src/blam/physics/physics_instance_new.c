/* physics_instance_new @ 0x837BDC40 — build the world-space physics matrix for an object so a ray can
 * be tested against its physics hull. Returns 0 if the object's definition has no physics
 * (object def +140 == -1). The matrix is assembled from the object's origin (translation) and
 * orientation (forward = row 0, up = row 2); the right vector (row 1) is the cross product up x
 * forward; finally the translation is shifted so the matrix is centred on the object's centre of
 * mass.
 *
 * Matrix rows: n[0]=forward, n[1]=right, n[2]=up, n[3]=translation. */

#include <stdint.h>
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

uint8_t physics_instance_new(physics_instance *instance, int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_def = TAG_GET(_object_definition, object->definition_index);
    real_matrix4x3 *m = &instance->world_matrix;
    const physics_definition *physics;
    real_point3d offset;

    if ( object_def->physics.index == -1 )
        return 0;

    instance->object_index = object_index;
    instance->physics = TAG_GET(const physics_definition, object_def->physics.index);
    m->scale = 1.0f;
    object_get_origin(object_index, (real_point3d *)m->n[3]);
    object_get_orientation(object_index, (real_vector3d *)m->n[0], (real_vector3d *)m->n[2]);

    /* right = up x forward */
    m->n[1][2] = m->n[2][0] * m->n[0][1] - m->n[0][0] * m->n[2][1];
    m->n[1][0] = m->n[0][2] * m->n[2][1] - m->n[2][2] * m->n[0][1];
    m->n[1][1] = m->n[2][2] * m->n[0][0] - m->n[0][2] * m->n[2][0];

    /* translate so the matrix origin is the object's centre of mass */
    physics = instance->physics;
    offset.n[0] = -physics->center_of_mass.n[0];
    offset.n[1] = -physics->center_of_mass.n[1];
    offset.n[2] = -physics->center_of_mass.n[2];
    matrix4x3_transform_point(m, &offset, &offset);
    m->n[3][0] = offset.n[0];
    m->n[3][1] = offset.n[1];
    m->n[3][2] = offset.n[2];
    return 1;
}
