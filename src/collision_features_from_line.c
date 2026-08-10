/* collision_features_from_line @0x83804220 — turn a line segment (base `point`, direction `vector`, radius
 * `width`, optional vertical extent `height`) into collision features: a cylinder along the line, and when
 * height>0 a second (lowered) cylinder plus two prisms forming the swept vertical wall (front and back faces).
 *
 * Deviation: identical float-GPR-skip scramble to collision_features_from_point — the Hex-Rays parameter names
 * are shifted by one and `features` arrives on the stack (decompiler `a32`). True register map (from the
 * prologue): r3=point, r4=vector, f1=height, f2=width, r7=object_index, r8=surface_index, r9=flags,
 * r10=breakable_surface_index, material_index/features on the stack. `vector` is real_vector3d (the decompiler
 * mis-typed it real_vector2d, reading the z component as vector[1].n[0]). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_vector2d.h"
#include "headers/collision_feature_list.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float);
extern double __fabs(double x);
extern real_vector2d *perpendicular2d(const real_vector2d *a, real_vector2d *result);
extern int16_t projection_from_vector3d(const real_vector3d *n);

static void project_prism_points(collision_prism *prism, const float corners[12])
{
    for ( int i = 0; i < prism->point_count; ++i )
    {
        const float *corner = &corners[3 * i];
        prism->points[i].n[0] = corner[global_projection3d_mappings[prism->projection_axis][prism->projection_sign][0]];
        prism->points[i].n[1] = corner[global_projection3d_mappings[prism->projection_axis][prism->projection_sign][1]];
    }
}

void collision_features_from_line(const real_point3d *point, const real_vector3d *vector, float height,
                                  float width, int object_index, int surface_index, uint8_t flags,
                                  uint8_t breakable_surface_index, int16_t material_index,
                                  collision_feature_list *features)
{
    if ( features->count[1] < 256 )
    {
        collision_cylinder *cylinder = &features->cylinders[features->count[1]++];
        cylinder->object_index = object_index;
        cylinder->surface_index = surface_index;
        cylinder->flags = flags;
        cylinder->breakable_surface_index = breakable_surface_index;
        cylinder->material_index = material_index;
        cylinder->base = *point;
        cylinder->height.n[0] = vector->n[0];
        cylinder->height.n[1] = vector->n[1];
        cylinder->height.n[2] = vector->n[2];
        cylinder->width = width;
    }

    if ( height <= 0.0 )
        return;

    float lowered_z = (point->n[2] - height);

    if ( features->count[1] < 256 )
    {
        collision_cylinder *cylinder = &features->cylinders[features->count[1]++];
        cylinder->object_index = object_index;
        cylinder->surface_index = surface_index;
        cylinder->flags = flags;
        cylinder->breakable_surface_index = breakable_surface_index;
        cylinder->material_index = material_index;
        cylinder->base.n[0] = point->n[0];
        cylinder->base.n[1] = point->n[1];
        cylinder->base.n[2] = lowered_z;
        cylinder->height.n[0] = vector->n[0];
        cylinder->height.n[1] = vector->n[1];
        cylinder->height.n[2] = vector->n[2];
        cylinder->width = width;
    }

    real_vector2d perpendicular;
    perpendicular2d((const real_vector2d *)vector, &perpendicular);
    float length = __fsqrts(((perpendicular.n[0] * perpendicular.n[0])
                                         + (perpendicular.n[1] * perpendicular.n[1])));
    if ( __fabs(length) < 0.000099999997 )
        return;

    char length_is_zero = (length == 0.0);
    float inverse_length = ((float)1.0 / length);
    perpendicular.n[0] = (inverse_length * perpendicular.n[0]);
    perpendicular.n[1] = (inverse_length * perpendicular.n[1]);
    if ( length_is_zero )
        return;

    float far_x = (point->n[0] + vector->n[0]);
    float far_y = (point->n[1] + vector->n[1]);
    float far_z = (point->n[2] + vector->n[2]);
    float distance = ((point->n[0] * perpendicular.n[0]) + (point->n[1] * perpendicular.n[1]));

    /* swept-wall quad corners (4 corners x 3 components), front-face winding */
    float corners[12];
    corners[0] = point->n[0]; corners[1] = point->n[1];  corners[2]  = point->n[2];
    corners[3] = far_x;       corners[4] = far_y;        corners[5]  = far_z;
    corners[6] = far_x;       corners[7] = far_y;        corners[8]  = (far_z - height);
    corners[9] = point->n[0]; corners[10] = point->n[1]; corners[11] = lowered_z;

    /* front prism (+perpendicular) */
    if ( features->count[2] < 256 )
    {
        collision_prism *prism = &features->prisms[features->count[2]++];
        prism->object_index = object_index;
        prism->surface_index = surface_index;
        prism->flags = flags;
        prism->breakable_surface_index = breakable_surface_index;
        prism->material_index = material_index;
        prism->plane.normal.n[0] = perpendicular.n[0];
        prism->plane.normal.n[1] = perpendicular.n[1];
        prism->plane.normal.n[2] = 0.0;
        prism->plane.distance = distance;
        prism->height = width;
        prism->projection_axis = projection_from_vector3d(&prism->plane.normal);
        prism->projection_sign = (prism->plane.normal.n[prism->projection_axis] > 0.0);
        prism->point_count = 4;
        project_prism_points(prism, corners);
    }

    /* back prism (-perpendicular), reversed winding */
    corners[3] = point->n[0]; corners[4] = point->n[1];  corners[5]  = lowered_z;
    corners[9] = far_x;       corners[10] = far_y;       corners[11] = far_z;

    if ( features->count[2] < 256 )
    {
        collision_prism *prism = &features->prisms[features->count[2]++];
        prism->object_index = object_index;
        prism->surface_index = surface_index;
        prism->flags = flags;
        prism->breakable_surface_index = breakable_surface_index;
        prism->material_index = material_index;
        prism->plane.normal.n[0] = -perpendicular.n[0];
        prism->plane.normal.n[1] = -perpendicular.n[1];
        prism->plane.normal.n[2] = 0.0;
        prism->plane.distance = -distance;
        prism->height = width;
        /* projection axis chosen by largest |normal component| (decompiler inlines the abs-max here rather
         * than calling projection_from_vector3d) */
        float abs_z = __fabs(prism->plane.normal.n[2]);
        float abs_y = __fabs(prism->plane.normal.n[1]);
        float abs_x = __fabs(prism->plane.normal.n[0]);
        prism->projection_axis = (abs_z < abs_y || abs_z < abs_x) ? (abs_y >= abs_x) : 2;
        prism->projection_sign = (prism->plane.normal.n[prism->projection_axis] > 0.0);
        prism->point_count = 4;
        project_prism_points(prism, corners);
    }
}
