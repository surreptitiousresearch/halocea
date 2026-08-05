/* objects_in_sphere @0x836F0DD0 — collect the objects whose bounding sphere intersects a query sphere.
 * Gathers the structure clusters touching the sphere, expands those to candidate objects of the requested
 * class flags, then keeps the ones whose (origin, bounding-radius) sphere overlaps the query sphere and
 * whose object type passes the type-flags mask. Writes accepted object indices to object_indices (capped
 * at maximum_count) and returns the number written.
 *
 * deviation: an earlier reconstruction added a phantom `int definition` parameter. The disassembly reads
 * r3,r4,r5,r6,f1,r8,r9 only — r7 is never an incoming argument (it is overwritten with a stack address at
 * 0x836F0E10 before any read); r7 is the GPR shadow slot skipped by the float `radius` arg (f1). So there
 * are 7 params: r8=object_indices, r9=maximum_count. Callers (area_of_effect_cause_damage @0x836B4D18)
 * set r9=0x40 and r8=buffer, confirming the 7-arg shape. maximum_count is sign-extended (extsh r31,r30
 * @0x836F0E50) → int16_t. Return is extsh'd by all 9 callers → int16_t. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern int16_t structure_clusters_in_sphere(int16_t cluster_index, const real_point3d *position, float radius, int16_t maximum_count, int16_t *intersected_indices);
extern int16_t objects_in_clusters_by_indices(unsigned int class_flags, int16_t cluster_count, const int16_t *cluster_indices, int16_t maximum_object_count, int *object_indices);

int16_t objects_in_sphere(unsigned int class_flags, unsigned int type_flags, const location *location,
        const real_point3d *center, float radius, int *object_indices, int16_t maximum_count)
{
    int16_t cluster_indices[512];
    int found_objects[2048];
    unsigned int type_mask = type_flags ? type_flags : 0xFFFFFFFF;
    int accepted = 0;
    int cluster_count;
    int object_count;
    int i;

    cluster_count = structure_clusters_in_sphere(location->cluster_index, center, radius,
            512, cluster_indices);
    object_count = objects_in_clusters_by_indices(class_flags, cluster_count, cluster_indices,
            2048, found_objects);

    for ( i = 0; i < object_count; ++i )
    {
        int object_index;
        object_datum *object;

        if ( accepted >= maximum_count )
            break;

        object_index = found_objects[i];
        object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

        if ( ((1 << (uint16_t)object->object.type) & type_mask) != 0 )
        {
            float dx = object->object.bounding_sphere_center.n[0] - center->n[0];
            float dy = object->object.bounding_sphere_center.n[1] - center->n[1];
            float dz = object->object.bounding_sphere_center.n[2] - center->n[2];
            float reach = object->object.bounding_sphere_radius + radius;

            if ( dx * dx + dy * dy + dz * dz <= reach * reach )
                object_indices[accepted++] = object_index;
        }
    }
    return accepted;
}
