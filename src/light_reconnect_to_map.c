/* light_reconnect_to_map @ 0x836F8880 — recompute a dynamic light's world position/orientation from its
 * owner object and re-insert it into the structure BSP's light cluster partition. Three cases:
 *   - light attached to an object marker (light int[22] == -1): resolve the marker transform by name and
 *     copy its position (row3), forward (row0) and up (row2) into the light's cached transform (int[12..20]);
 *   - light attached to an object node (owner still valid): transform the light's local origin/direction
 *     through the node matrix, then build a perpendicular up vector;
 *   - otherwise leave the cached transform as-is.
 * Then, if the light is map-relevant (flags bit1), compute its bounding sphere, resolve a scenario or
 * object location for it, and reconnect it to the partition (setting the "connected" bit2). Light datum
 * stride 124 bytes; owner object index at int[11], marker/node index at short[46], partition link at int[4]. */

#include <stdint.h>
#include "headers/light_datum.h"
#include "headers/data_array.h"
#include "headers/object_marker.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_type.h"
#include "headers/point_light_flags.h"
#include "headers/blam_data_globals.h"

struct cluster_partition;

#include "headers/cluster_partition.h"
extern const char *object_get_attachment_marker_name(int object_index, int16_t attachment_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern float normalize3d(real_vector3d *v);
extern void light_compute_bounding_sphere(uint16_t light_index, uint8_t maximum, uint8_t specular, uint8_t lens_flare_only, real_point3d *bounding_sphere_center, float *bounding_sphere_radius);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern location *object_get_location(int object_index, location *location_out);
extern void cluster_partition_reconnect(cluster_partition *partition, int datum_index, int *first_cluster_reference, const real_point3d *position, float radius, const location *location);

void light_reconnect_to_map(uint16_t light_index)
{
    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
    int owner_object_index = light->object_index;

    if ( light->parent_light_index == -1 )
    {
        const char *attachment_marker_name =
            object_get_attachment_marker_name(owner_object_index, (unsigned __int16)light->attachment_marker_index);
        object_marker marker;
        object_get_marker_by_name(light->object_index, attachment_marker_name, &marker, 1);

        light->position.n[0] = marker.matrix.n[3][0];   /* position */
        light->position.n[1] = marker.matrix.n[3][1];
        light->position.n[2] = marker.matrix.n[3][2];
        light->forward.n[0] = marker.matrix.n[0][0];   /* forward */
        light->forward.n[1] = marker.matrix.n[0][1];
        light->forward.n[2] = marker.matrix.n[0][2];
        light->up.n[0] = marker.matrix.n[2][0];   /* up */
        light->up.n[1] = marker.matrix.n[2][1];
        light->up.n[2] = marker.matrix.n[2][2];
    }
    else if ( object_try_and_get_and_verify_type(owner_object_index, object_mask_all) )
    {
        const real_matrix4x3 *node_matrix =
            object_get_node_matrix(light->object_index, (unsigned __int16)light->attachment_marker_index);
        /* BUGFIX vs prior source: the decompiler's (real_point3d*)v2+8 etc. are 12-byte scaled
         * (bytes 0x60/0x6C in, 0x30/0x3C/0x48 out); the old transcription int-scaled them (0x20...). */
        matrix4x3_transform_point(node_matrix, &light->relative_position, &light->position);
        matrix4x3_transform_normal(node_matrix, &light->relative_forward, &light->forward);
        perpendicular3d(&light->forward, &light->up);
        normalize3d(&light->up);
    }

    if ( (light->flags & (1u << _point_light_connects_to_map_bit)) != 0 )
    {
        real_point3d bounding_sphere_center;
        float bounding_sphere_radius[2];
        location light_location;

        light_compute_bounding_sphere(light_index, 1u, 0, 1u, &bounding_sphere_center, bounding_sphere_radius);

        int owner = light->object_index;
        if ( owner == -1 || !object_try_and_get_and_verify_type(owner, object_mask_all) )
            scenario_location_from_point(&light_location, &bounding_sphere_center);
        else
            object_get_location(light->object_index, &light_location);

        cluster_partition_reconnect(&light_cluster_partition, light_index, &light->cluster_reference,
            &bounding_sphere_center, bounding_sphere_radius[0], &light_location);
        light->flags |= (1u << _point_light_connected_to_map_bit);
    }
}
