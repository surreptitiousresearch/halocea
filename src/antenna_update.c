/* antenna_update @0x8380D8F8 — advance one antenna's spring simulation by dt. First it re-evaluates the
 * attachment (base) location/point/direction from the antenna's host object. Then, for each of the
 * (vertices.count + 1) nodes, it integrates the node through point_physics_update (gravity/drag), pulls it
 * back to the definition's rest segment length from the previous node, blends the result toward the prior
 * node's swaying tip by the spring coefficient, orients the node's sprite direction by rotating the
 * definition's cached vector about the perpendicular of the segment, and records the node's per-frame
 * velocity. The root node (index 0) is pinned to the attachment point/vector. Nodes that produce a
 * degenerate perpendicular fall back to global_left3d.
 *
 * Reconstructed from disassembly (0x8380D8F8-0x8380DC68); the decompiler emitted "local variable
 * allocation has failed" and rendered the loop-carried vectors as OVERLAPPED/__PAIR64 register puns. The
 * root-node branch is a plain 3-float copy of the attachment point/vector (captured into locals before the
 * loop, since the loop reuses those stack slots as scratch). cos/sin appear as long-double scratch aliases
 * of the angle; restored to plain double calls. point_physics_update's 9th arg (collision_material_type)
 * is nullptr. The loop-carried tip (v12/v10/v8) is read uninitialized on entry but is dead until node 1
 * (node 0 always reassigns it), so it is seeded to 0. */

#include <stdint.h>
#include "headers/antenna_datum.h"
#include "headers/antenna_definition.h"
#include "headers/antenna_vertex_datum.h"
#include "headers/antenna_vertex.h"
#include "headers/point_physics_definition.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/global_tag_instances.h"
#include <math.h>
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern double __fabs(double x);
extern void antenna_update_attachment(antenna_datum *antenna, antenna_definition *antenna_definition, location *attachment_location, real_point3d *attachment_point, real_vector3d *attachment_vector);
extern float angle_between_vectors3d(const real_vector3d *a, const real_vector3d *b);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern unsigned int point_physics_update(unsigned int flags, const point_physics_definition *definition, location *location, int16_t force_weather_palette_index, real_point3d *position, real_vector3d *translational_velocity, const real_vector3d *translational_force, real_vector3d *collision_normal, int16_t *collision_material_type, float radius, float dt);

void antenna_update(antenna_datum *antenna, antenna_definition *antenna_definition, float dt)
{
    location attachment_location;
    real_point3d attachment_point;
    real_vector3d attachment_vector;
    antenna_update_attachment(antenna, antenna_definition, &attachment_location,
            &attachment_point, &attachment_vector);

    if ( antenna->__noop || dt <= 0.0f )
        return;

    int count = antenna_definition->vertices.count;
    if ( count + 1 <= 0 )
        return;

    antenna_vertex_datum *vertices = antenna->vertices;
    float inverse_dt = (1.0f / dt);

    /* loop-carried previous-node swaying tip (dead until node 1) */
    float carried_x = 0.0f;
    float carried_y = 0.0f;
    float carried_z = 0.0f;

    real_point3d position;
    real_vector3d segment;
    real_vector3d previous_position;

    int vertex_index = 0;
    do
    {
        antenna_vertex_datum *vertex = &vertices[vertex_index];
        int clamped_index = (vertex_index == count) ? (count - 1) : vertex_index;
        antenna_vertex *definition_vertex =
            &((antenna_vertex *)antenna_definition->vertices.address)[clamped_index];
        float rest_length = definition_vertex->spring_coefficient;   /* per-vertex spring @+0 */
        ++vertex->sprite_index;
        float spring = (antenna_definition->spring_coefficient * rest_length);

        if ( vertex_index == 0 )
        {
            position = attachment_point;
            segment = attachment_vector;
        }
        else
        {
            /* prior hand-source added +0x14 to TAG_INSTANCE (which already points at base_address),
             * landing past the element; DB reads element[5] == base_address */
            const point_physics_definition *definition =
                    TAG_GET(const point_physics_definition, antenna_definition->physics.index);

            position.n[0] = vertex->p.n[0];
            position.n[1] = vertex->p.n[1];
            position.n[2] = vertex->p.n[2];
            point_physics_update(0, definition, &attachment_location, -1, &position,
                    &vertex->v, nullptr, nullptr, nullptr, 0.02f, dt);

            float delta_x = (position.n[0] - previous_position.n[0]);
            float delta_y = (position.n[1] - previous_position.n[1]);
            float delta_z = (position.n[2] - previous_position.n[2]);
            float distance = __fsqrts(((delta_x * delta_x)
                    + ((delta_z * delta_z) + (delta_y * delta_y))));
            float scale = (definition_vertex->length_to_next / distance);  /* was float-index [9] = +0x24 */

            float target_x = ((scale * delta_x) + previous_position.n[0]);
            float target_y = ((scale * delta_y) + previous_position.n[1]);
            float target_z = ((scale * delta_z) + previous_position.n[2]);

            float new_x = (((1.0f - spring) * target_x) + (carried_x * spring));
            position.n[1] = (((1.0f - spring) * target_y) + (carried_y * spring));
            position.n[2] = (((1.0f - spring) * target_z) + (carried_z * spring));
            position.n[0] = new_x;

            segment.n[0] = (new_x - previous_position.n[0]);
            segment.n[1] = (position.n[1] - previous_position.n[1]);
            segment.n[2] = (position.n[2] - previous_position.n[2]);
        }

        /* build the per-node sprite orientation frame: perpendicular = up x segment */
        real_vector3d up_axis;
        up_axis.n[0] = 0.0f;
        up_axis.n[1] = 0.0f;
        up_axis.n[2] = 1.0f;

        real_vector3d perpendicular;
        perpendicular.n[2] = ((segment.n[1] * 0.0f) - (segment.n[0] * 0.0f));
        perpendicular.n[0] = ((segment.n[2] * 0.0f) - segment.n[1]);
        perpendicular.n[1] = (segment.n[0] - (segment.n[2] * 0.0f));

        float perpendicular_length = __fsqrts(((perpendicular.n[1] * perpendicular.n[1])
                + ((perpendicular.n[2] * perpendicular.n[2])
                        + (perpendicular.n[0] * perpendicular.n[0]))));
        if ( __fabs(perpendicular_length) < 0.000099999997f )
        {
            perpendicular = *global_left3d;
        }
        else
        {
            float inverse_length = (1.0f / perpendicular_length);
            perpendicular.n[0] = (inverse_length * perpendicular.n[0]);
            perpendicular.n[1] = (inverse_length * perpendicular.n[1]);
            perpendicular.n[2] = (perpendicular.n[2] * inverse_length);
            if ( perpendicular_length == 0.0f )
                perpendicular = *global_left3d;
        }

        real_vector3d sprite_vector;
        sprite_vector.n[0] = definition_vertex->vector_to_next.n[0];  /* was float-index [29..31] = +0x74 */
        sprite_vector.n[1] = definition_vertex->vector_to_next.n[1];
        sprite_vector.n[2] = definition_vertex->vector_to_next.n[2];
        float angle = angle_between_vectors3d(&up_axis, &segment);
        float cosine = (float)cos(angle);
        float sine = (float)sin(angle);
        rotate_vector_about_axis(&sprite_vector, &perpendicular, sine, cosine);

        float old_x = vertex->p.n[0];
        float old_y = vertex->p.n[1];
        float old_z = vertex->p.n[2];
        float velocity_x = (position.n[0] - old_x);
        float velocity_y = (position.n[1] - old_y);
        float velocity_z = (position.n[2] - old_z);

        carried_x = (sprite_vector.n[0] + position.n[0]);
        carried_y = (sprite_vector.n[1] + position.n[1]);
        carried_z = (sprite_vector.n[2] + position.n[2]);

        previous_position.n[0] = position.n[0];
        previous_position.n[1] = position.n[1];
        previous_position.n[2] = position.n[2];

        vertex->p.n[0] = position.n[0];
        vertex->p.n[1] = position.n[1];
        vertex->p.n[2] = position.n[2];
        vertex->v.n[0] = (velocity_x * inverse_dt);
        vertex->v.n[1] = (velocity_y * inverse_dt);
        vertex->v.n[2] = (velocity_z * inverse_dt);

        vertex_index = (int16_t)(vertex_index + 1);
        count = antenna_definition->vertices.count;
    }
    while ( vertex_index < count + 1 );
}
