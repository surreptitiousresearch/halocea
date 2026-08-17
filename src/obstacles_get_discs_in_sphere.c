/* obstacles_get_discs_in_sphere @0x8382FA40 — populates `obstacles` with one disc per marker-defined obstacle
 * point found on every object within `radius` of `center` (excluding the two given objects), gathered via a
 * coarse `objects_in_sphere` query seeded from `ignore_source_object_index`'s cluster/leaf location. For each
 * candidate object whose bounding sphere overlaps the query sphere, its model tag's marker block is walked;
 * each marker becomes a disc positioned at the marker's world-space transform (node-relative if the marker is
 * attached to a node, object-relative otherwise), sized by the marker's own radius field scaled by the
 * transform's uniform scale. A disc is flagged "optional" (bit 0) when the candidate's own velocity has a
 * strong-enough positive dot product against `movement_direction` (i.e. the obstacle is already receding from
 * the path) — this is only evaluated for type-0 (biped) candidates whose marker point also lies in front of the
 * mover along `movement_direction`.
 *
 * DEVIATION — same dead-float-shadow-GPR pattern as path_avoid_obstacles/path_find: `radius` (param 3, float)
 * consumes a positional GPR shadow (r5, verified dead in the prologue via disasm_range 0x8382FA40-0x8382FB70),
 * shifting every later int/pointer parameter's Hex-Rays name one slot out of alignment with its real binding:
 *   - decompile's declared param 4 `movement_direction` (real_vector3d*) is DEAD — never dereferenced in the
 *     body. The REAL movement_direction data is read through decompile's param 5, mis-typed/mis-named
 *     `ignore_source_object_index` (float*) — confirmed bound to r22 (`mr r22, r6` in the prologue; r6 is the
 *     true param-4 register) and dereferenced as a 3-float vector throughout (0x8382FCC4, 0x8382FCE0,
 *     0x8382FD38-0x8382FD44).
 *   - decompile's first `ignore_target_object_index` comparison (0x8382FB14, `cmpw cr6, r29, r26`) is bound to
 *     r26 (`mr r26, r7` in the prologue; r7 is the true param-5 register) — this is really the caller's
 *     `ignore_source_object_index`.
 *   - decompile's phantom trailing param `a7` (0x8382FB30, `cmpw cr6, r29, r18`) is bound to r18 (`mr r18, r8`;
 *     r8 is the true param-6 register) — this is really the caller's `ignore_target_object_index`.
 * All four renamed below to their real DB-prototype bindings; `obstacles`/`center`/`radius` were already
 * correctly bound and are unchanged.
 *
 * DEVIATION — the `objects_in_sphere` call's own outgoing args are scrambled by the same shadow-slot pattern
 * (its `radius` param also owns a dead shadow): decompile passes its `v13` (an object-header lookup pointer,
 * used only to derive `location`) into the visible "object_indices" slot, but disasm_range (0x8382FA88-
 * 0x8382FAA8) shows the real `object_indices`/`maximum_count` outgoing registers (r8/r9) are the stack scratch
 * array and the literal 0x100 (256) — `v13`/r7 is the dead shadow slot, coincidentally still holding a leftover
 * computed value that is never read by the callee. Modeled with the real 256-entry `int` output array (DB
 * prototype: `int *object_indices`) rather than reusing the decompile's stray `v13`.
 *
 * Object fields are typed via object_datum/unit_datum; the candidate's collision_model.pathfinding_spheres
 * block supplies the disc positions. NOTE: pathfinding_sphere.radius (@28) and node_index (@0) are
 * DB-confirmed, but the code reads the sphere point at +8 whereas the DB labels center at +16; the +8 read
 * is preserved raw pending confirmation. */

#include <stdint.h>
#include "headers/obstacles.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/location.h"
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/device_datum.h"
#include "headers/_object_definition.h"
#include "headers/machine_definition.h"
#include "headers/object_type.h"
#include "headers/collision_model.h"
#include "headers/pathfinding_sphere.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/object_definition_flags.h"
#include "headers/unit_control_flags.h"
#include "headers/machine_flags.h"
#include "headers/blam_data_globals.h"

extern int16_t objects_in_sphere(unsigned int class_flags, unsigned int type_flags, const location *location, const real_point3d *center, float radius, int *object_indices, int16_t maximum_count);
extern real_matrix4x3 *object_get_world_matrix(int object_index, real_matrix4x3 *matrix);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern int obstacles_add_disc(obstacles *obstacles, int object_index, int16_t flags, const real_point3d *center, float radius);

void obstacles_get_discs_in_sphere(obstacles *obstacles, const real_point3d *center, float radius,
        const real_vector3d *movement_direction, int ignore_source_object_index, int ignore_target_object_index)
{
    object_datum *source_object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, ignore_source_object_index)->datum;
    const location *source_location = &source_object->object.location;

    int object_indices[256];
    int16_t found_count = objects_in_sphere(1, object_mask_sightblocking | object_mask_biped,   /* 0xC3 */ source_location, center, radius, object_indices, 256);

    for (int16_t i = 0; i < found_count; i++)
    {
        int candidate_index = object_indices[i];
        if (candidate_index == ignore_source_object_index || candidate_index == ignore_target_object_index)
            continue;

        object_datum *candidate_object =
            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, candidate_index)->datum;

        if ((candidate_object->object.flags & (1u << _object_invisible_bit)) != 0)
            continue;

        int16_t candidate_type = candidate_object->object.type;
        /* DEVIATION: dead test applies only to bipeds (type 0) and rejects the DEAD ones —
         * bne cr6,0x8382FB68 @0x8382FB54 skips the test for type!=0; rlwinm(dead bit)+bne
         * @0x8382FB5C-0x8382FB64 rejects when set. Prior recovery had it inverted and gating
         * non-bipeds. */
        if (candidate_type == 0 && (candidate_object->object.damage_flags & (1u << _object_dead_bit)) != 0)
            continue;

        if (candidate_type == object_type_machine)
        {
            /* machine-type candidates are additionally gated by the machine tag flags (machine_definition+658)
             * and, when bit 1 is set, an offset-520 float that must not equal 1.0 */
            int16_t machine_flags = TAG_GET(machine_definition, candidate_object->definition_index)->machine.flags;
            if ((machine_flags & (1u << _machine_is_pathfinding_obstacle_bit)) == 0)
                continue;
            /* DEVIATION: decompiler punned this float read (offset 520) through unit.control_flags;
             * on a machine object it is device_datum.device.position (float, DB-verified) — fully open. */
            if ((machine_flags & (1u << _machine_is_not_pathfinding_obstacle_when_open_bit)) != 0
              && ((device_datum *)candidate_object)->device.position == 1.0f)
                continue;
        }

        float dz = candidate_object->object.bounding_sphere_center.n[2] - center->n[2];
        float dx = candidate_object->object.bounding_sphere_center.n[0] - center->n[0];
        float bounding_radius_plus_query = candidate_object->object.bounding_sphere_radius + radius;
        float dy = candidate_object->object.bounding_sphere_center.n[1] - center->n[1];
        if (dx * dx + dy * dy + dz * dz > bounding_radius_plus_query * bounding_radius_plus_query)
            continue;

        _object_definition *object_definition_tag_data =
            TAG_GET(_object_definition, candidate_object->definition_index);
        collision_model *model_tag_data =
            TAG_GET(collision_model, object_definition_tag_data->collision_model.index);
        if ((object_definition_tag_data->flags & (1u << _object_not_pathfinding_obstacle_bit)) != 0
          || model_tag_data->pathfinding_spheres.count <= 0)
            continue;

        real_matrix4x3 world_matrix;
        object_get_world_matrix(candidate_index, &world_matrix);

        int marker_count = model_tag_data->pathfinding_spheres.count;
        pathfinding_sphere *marker_data = (pathfinding_sphere *)model_tag_data->pathfinding_spheres.address;

        for (int marker_index = 0; marker_index < marker_count; marker_index++)
        {
            pathfinding_sphere *marker = &marker_data[marker_index];
            int16_t marker_node_index = marker->node_index;

            real_point3d marker_world_point;
            float scale;
            if (marker_node_index == -1)
            {
                matrix4x3_transform_point(&world_matrix, &marker->center, &marker_world_point);
                scale = world_matrix.scale;
            }
            else
            {
                const real_matrix4x3 *node_matrix = object_get_node_matrix(candidate_index, marker_node_index);
                matrix4x3_transform_point(node_matrix, &marker->center, &marker_world_point);
                scale = node_matrix->scale;
            }

            float marker_radius = marker->radius * scale;

            uint8_t within_z_bounds =
                    (marker_world_point.n[2] + marker_radius + 0.5f >= center->n[2]
                            || movement_direction->n[2] <= -0.2f)
                    && (marker_world_point.n[2] - marker_radius - 0.5f <= center->n[2]
                            || movement_direction->n[2] >= 0.2f);
            if (!within_z_bounds)
                continue;

            float marker_dx = marker_world_point.n[0] - center->n[0];
            float marker_dy = marker_world_point.n[1] - center->n[1];
            float marker_dz = marker_world_point.n[2] - center->n[2];
            float total_radius = marker_radius + radius;

            if (marker_dx * marker_dx + marker_dz * marker_dz * 4.0f + marker_dy * marker_dy
                    > total_radius * total_radius)
                continue;

            uint8_t optional = 0;
            if (candidate_type == 0
                    && (movement_direction->n[0] * marker_dx + movement_direction->n[1] * marker_dy
                            + movement_direction->n[2] * marker_dz) > 0.0f)
            {
                float candidate_velocity_x = candidate_object->object.translational_velocity.n[0];
                float candidate_velocity_y = candidate_object->object.translational_velocity.n[1];
                float candidate_velocity_z = candidate_object->object.translational_velocity.n[2];
                optional = (movement_direction->n[0] * candidate_velocity_x
                        + candidate_velocity_y * movement_direction->n[1]
                        + candidate_velocity_z * movement_direction->n[2]) > 0.06666667f;
            }

            obstacles_add_disc(obstacles, candidate_index, optional, &marker_world_point, marker_radius);
        }
    }
}
