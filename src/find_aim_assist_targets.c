/* find_aim_assist_targets @0x836C6200 — gather candidate auto-aim / magnetism targets along an aiming ray.
 * Picks the larger of the autoaim/magnetism distance and angle, bails if either is non-positive, then
 * finds all structure clusters inside the resulting cone, collects the objects in those clusters, and runs
 * each object through find_aim_assist_targets_recursive to accumulate up to maximum_target_count results.
 * Returns the number of targets written to `targets`.
 *
 * Reconstructed against the disassembly: the decompiler mangled this through the FPR-shadow ABI (the three
 * float args distance/sine/cosine each reserve a GPR slot), losing the cluster/object scratch-buffer sizes
 * and shifting the recursive call's trailing args. The cluster buffer holds 512 indices, the object buffer
 * 2048 indices, and targets are written as &targets[total] with stride sizeof(aim_assist_target). */

#include <stdint.h>
#include "headers/aim_assist_parameters.h"
#include "headers/aim_assist_target.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"

extern double sin(double x);
extern double cos(double x);
extern int structure_clusters_in_cone(int16_t position_cluster_index, const real_point3d *position, const real_vector3d *direction, float distance, float sine, float cosine, int16_t maximum_cluster_count, int16_t *cluster_indices);
extern int16_t objects_in_clusters_by_indices(unsigned int class_flags, int16_t cluster_count, const int16_t *cluster_indices, int16_t maximum_object_count, int *object_indices);
extern int16_t find_aim_assist_targets_recursive(const aim_assist_parameters *parameters, int object_index, const real_point3d *position, const real_vector3d *direction, float distance, float angle_sine, float angle_cosine, int ignore_object_index, int16_t ignore_team_index, int16_t maximum_target_count, aim_assist_target *targets);

int16_t find_aim_assist_targets(const aim_assist_parameters *parameters, int16_t position_cluster_index, const real_point3d *position, const real_vector3d *direction, aim_assist_target *ignore_object_index, int16_t ignore_team_index, int16_t maximum_target_count, aim_assist_target *targets)
{
    float cone_distance = parameters->magnetism_distance;
    if (parameters->autoaim_distance > cone_distance)
        cone_distance = parameters->autoaim_distance;

    float cone_angle = parameters->autoaim_angle;
    if (cone_angle <= parameters->magnetism_angle)
        cone_angle = parameters->magnetism_angle;

    if (cone_distance <= 0.0)
        return 0;
    if (cone_angle <= 0.0)
        return 0;

    float angle_sine = (float)sin(cone_angle);
    float angle_cosine = (float)cos(cone_angle);

    int16_t cluster_indices[512];
    int object_indices[2048];
    int16_t cluster_count = structure_clusters_in_cone(position_cluster_index, position, direction,
                                                       cone_distance, angle_sine, angle_cosine, 512,
                                                       cluster_indices);
    int16_t object_count = objects_in_clusters_by_indices(1u, cluster_count, cluster_indices, 2048,
                                                          object_indices);
    if (object_count <= 0)
        return 0;

    int total_found = 0;
    for (int16_t i = 0; i < object_count; i++)
    {
        int found = find_aim_assist_targets_recursive(parameters, object_indices[i], position, direction,
                                                       cone_distance, angle_sine, angle_cosine,
                                                       (int)ignore_object_index, ignore_team_index,
                                                       maximum_target_count - total_found,
                                                       &targets[total_found]);
        total_found = (int16_t)(found + total_found);
        if (total_found >= maximum_target_count)
            break;
    }
    return total_found;
}
