/* model_get_marker_by_name @0x83799050 — collect all instances of a named marker on a model, posing each
 * into world space. For every marker instance whose region/permutation passes the supplied filter, the
 * marker's local transform (point + quaternion) is built, concatenated with its node's world matrix, and
 * (optionally) Y-mirrored. Returns the number of markers written, capped at maximum_marker_count.
 *
 */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/model.h"
#include "headers/model_marker.h"
#include "headers/model_marker_instance.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_quaternion.h"

extern int16_t model_find_marker(int model_index, const char *name);
extern void matrix4x3_from_point_and_quaternion(real_matrix4x3 *matrix, const real_point3d *point, const real_quaternion *quaternion);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);

int16_t model_get_marker_by_name(int model_index, const char *name, const uint8_t *region_permutations,
        const int16_t *node_remapping_table, int16_t node_count, const real_matrix4x3 *node_matrices,
        uint8_t mirrored_flag, object_marker *markers, int16_t maximum_marker_count)
{
    int count = 0;
    __int16 marker_index = model_find_marker(model_index, name);
    const model_marker *marker_record;
    int instance;

    if ( marker_index == -1 )
        return 0;

    marker_record = &((const model_marker *)(TAG_GET(model, model_index))->markers.address)[marker_index];
    if ( marker_record->instances.count <= 0 )
        return 0;

    for ( instance = 0; instance < marker_record->instances.count; instance = (__int16)(instance + 1) )
    {
        const model_marker_instance *record =
            &((const model_marker_instance *)marker_record->instances.address)[instance];
        object_marker *out;

        if ( region_permutations && region_permutations[record->region_index] != record->permutation_index )
            continue;

        if ( (__int16)count >= maximum_marker_count )
            return count;
        out = &markers[(__int16)count];
        count = (__int16)(count + 1);

        out->node_index = node_remapping_table ? node_remapping_table[record->node_index] : record->node_index;
        matrix4x3_from_point_and_quaternion(&out->node_matrix, &record->translation, &record->rotation);
        matrix4x3_multiply(&node_matrices[out->node_index], &out->node_matrix, &out->matrix);
        if ( mirrored_flag )
        {
            out->matrix.n[1][0] = -out->matrix.n[1][0];
            out->matrix.n[1][1] = -out->matrix.n[1][1];
            out->matrix.n[1][2] = -out->matrix.n[1][2];
        }
    }
    return count;
}
