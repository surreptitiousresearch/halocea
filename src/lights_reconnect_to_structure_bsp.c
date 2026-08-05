/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* lights_reconnect_to_structure_bsp @0x836F8A18 — refresh every attached light's transform and cluster
 * membership after the structure BSP changed. Walks all live lights; for each light flagged as needing a
 * refresh (flag 0x4, which is cleared) it recomputes the light's world transform from its host object: either
 * by reading the named attachment marker's transform, or by transforming the light's object-space
 * position/direction through the host node matrix (rebuilding a perpendicular and renormalizing the up
 * vector). Lights additionally flagged dirty (0x2) then recompute their bounding sphere, resolve a location
 * (from the host object, or from the sphere center if unhosted), reconnect into the light cluster partition,
 * and set the "connected" flag (0x4). No value is returned: r3 at the blr is only the terminal -1 left by
 * the datum iterator, and the reconnect_to_structure_bsp_procs table types every handler void(*)(void).
 *
 * Light-datum fields (124-byte stride) are raw offsets: flags @+2, host object @+44, marker/node discriminator
 * @+88, marker/node index @+92, world position @+48, forward @+60, up @+72, object-space position @+96 and
 * direction @+108, cluster reference @+16. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/light_datum.h"
#include "headers/object_marker.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/location.h"
#include "headers/cluster_partition.h"
#include "headers/object_type.h"
#include "headers/point_light_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/cluster_partition.h"
#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern const char *object_get_attachment_marker_name(int object_index, int16_t attachment_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern float __fsqrts(double value);
extern double __fabs(double value);
extern void light_compute_bounding_sphere(uint16_t light_index, uint8_t maximum, uint8_t specular, uint8_t lens_flare_only, real_point3d *bounding_sphere_center, float *bounding_sphere_radius);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern location *object_get_location(int object_index, location *location_out);
extern void cluster_partition_reconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference, const real_point3d *position, float radius, const location *location);

void lights_reconnect_to_structure_bsp(void)
{
    int result = data_next_index(light_data, -1);
    for ( int i = result; result != -1; i = result )
    {
        light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, i);
        int16_t flags = light->flags;
        if ( (flags & (1u << _point_light_connected_to_map_bit)) != 0 )
        {
            light->flags = flags & ~(1u << _point_light_connected_to_map_bit);
            int host_object = light->object_index;

            if ( light->parent_light_index == -1 )   /* marker-attached (vs node-attached) discriminator */
            {
                const char *marker_name = object_get_attachment_marker_name(host_object,
                        (uint16_t)light->attachment_marker_index);
                object_marker marker;
                object_get_marker_by_name(light->object_index, marker_name, &marker, 1);
                light->position.n[0] = marker.matrix.n[3][0];
                light->position.n[1] = marker.matrix.n[3][1];
                light->position.n[2] = marker.matrix.n[3][2];
                light->forward.n[0] = marker.matrix.n[0][0];
                light->forward.n[1] = marker.matrix.n[0][1];
                light->forward.n[2] = marker.matrix.n[0][2];
                light->up.n[0] = marker.matrix.n[2][0];
                light->up.n[1] = marker.matrix.n[2][1];
                light->up.n[2] = marker.matrix.n[2][2];
            }
            else if ( object_try_and_get_and_verify_type(host_object, object_mask_all) )
            {
                const real_matrix4x3 *node_matrix = object_get_node_matrix(light->object_index,
                        (uint16_t)light->attachment_marker_index);
                matrix4x3_transform_point(node_matrix, &light->relative_position, &light->position);
                matrix4x3_transform_normal(node_matrix, &light->relative_forward, &light->forward);
                perpendicular3d(&light->forward, &light->up);

                float up_y = light->up.n[1];
                float up_z = light->up.n[2];
                float length = __fsqrts(light->up.n[2] * light->up.n[2]
                        + (light->up.n[0] * light->up.n[0]
                         + light->up.n[1] * light->up.n[1]));
                if ( __fabs(length) >= 0.000099999997 )
                {
                    float inverse = 1.0f / length;
                    light->up.n[0] = light->up.n[0] * inverse;
                    light->up.n[1] = up_y * inverse;
                    light->up.n[2] = up_z * inverse;
                }
            }

            if ( (light->flags & (1u << _point_light_connects_to_map_bit)) != 0 )
            {
                real_point3d bounding_center;
                float bounding_radius[2];
                light_compute_bounding_sphere(i, 1u, 0, 1u, &bounding_center, bounding_radius);

                location light_location;
                if ( host_object == -1 || !object_try_and_get_and_verify_type(host_object, object_mask_all) )
                    scenario_location_from_point(&light_location, &bounding_center);
                else
                    object_get_location(light->object_index, &light_location);

                cluster_partition_reconnect(&light_cluster_partition, i, &light->cluster_reference,
                        &bounding_center, bounding_radius[0], &light_location);
                light->flags |= (1u << _point_light_connected_to_map_bit);
            }
        }
        result = data_next_index(light_data, i);
    }
}
