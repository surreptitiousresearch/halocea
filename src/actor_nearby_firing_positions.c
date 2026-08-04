/* actor_nearby_firing_positions @0x837EFAC0 — does the actor's encounter have a usable firing position close to
 * a test point? Selects the firing-position group bitmask for the current selection mode, and (when not using
 * the fast BSP path test) builds a path-state search from the test point. It then scans the encounter's firing
 * positions: for each one in the selected group and within 4 world units of the test point, it confirms
 * reachability — either a direct 3D BSP path test (path-search actors) or an A* estimated distance under 4
 * units — and returns 1 on the first hit. Returns 0 if none qualify.
 *
 * DEVIATION: the DB 4-arg prototype is authoritative; the decompiler's RtlCheckStack12 stack-probe misrender
 * fabricated a huge phantom-parameter pile (a5..a59) that is really this frame's stack scratch — a path_input,
 * a path_state, and a float distance out. Register-level disasm resolved the calls: path_input_new radius is the
 * actor definition's pathfinding_radius (+140), ignore_broken_surfaces=1, ignore_source_object_index=-1; the
 * firing-position group test is (1 << group_index) (r29=1, disasm 0x837EFBD8); and path_3d_available's float
 * avoidance_distance (arg 3) burns the r5 shadow, so its destination_reference is the firing position and its
 * two output args are null (same shape as path_3d_build_path). actor typed to name meta.definition_index/
 * meta.encounter_index; the actor definition, encounter (176-byte stride) and firing positions (24-byte stride)
 * are typed: encounter_definition (176 bytes, firing_positions tag_block @152) and its
 * firing_position_definition array (24 bytes: position @0, group_index @12, cluster_index @14, surface_index @20). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_definition.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/firing_position_definition.h"
#include "headers/real_point3d.h"
#include "headers/path_input.h"
#include "headers/path_state.h"
#include "headers/structure_bsp.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/blam_data_globals.h"

struct path_debug_storage;


#include "headers/path_debug_storage.h"
#include "headers/structure_bsp.h"
#include "headers/real_vector3d.h"
extern unsigned int actor_get_firing_position_group(uint16_t actor_index, int16_t evaluation_mode, int16_t group_selection_mode);
extern void path_input_new(path_input *input, float pathfinding_radius, uint8_t ignore_broken_surfaces, int ignore_source_object_index);
extern void path_input_set_start(path_input *input, const real_point3d *start_point, int start_surface_index);
extern void path_input_set_search_bounds(path_input *input, float maximum_distance);
extern void path_state_new(const path_input *input, path_state *state, path_debug_storage *debug);
extern uint8_t path_state_find(path_state *state);
extern int path_state_estimated_distance(path_state *state, const real_point3d *end_point, int end_surface_index, float *distance_reference, float *closest_approach_to_attractor_reference, real_vector3d *estimated_direction_reference);
extern uint8_t path_3d_available(structure_bsp *structure_bsp, const real_point3d *start_point, float avoidance_distance, const collision_bsp_test_vector_result *destination_reference, uint8_t *path_available_out, float *hit_result_out);

uint8_t actor_nearby_firing_positions(int actor_index, real_point3d *test_point, int test_surface_index, int16_t group_selection_mode)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned char use_bsp_path_test = actor->state.flying;
    actor_definition *actor_def = TAG_GET(actor_definition, actor->meta.definition_index);

    if ( (use_bsp_path_test || test_surface_index != -1) && actor->meta.encounter_index != -1 )
    {
        encounter_definition *encounter =
            &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned __int16)actor->meta.encounter_index];
        unsigned int firing_position_group = actor_get_firing_position_group(actor_index, 0, group_selection_mode);

        path_state state;
        if ( !use_bsp_path_test )
        {
            path_input input;
            path_input_new(&input, actor_def->moving.pathfinding_radius, 1, -1);
            path_input_set_start(&input, test_point, test_surface_index);
            path_input_set_search_bounds(&input, 4.0f);
            path_state_new(&input, &state, nullptr);
            path_state_find(&state);
        }

        int firing_position_count = encounter->firing_positions.count;
        if ( firing_position_count > 0 )
        {
            firing_position_definition *firing_positions =
                (firing_position_definition *)encounter->firing_positions.address;
            for ( int i = 0; i < firing_position_count; i = (__int16)(i + 1) )
            {
                firing_position_definition *firing_position = &firing_positions[i];
                if ( ((1 << firing_position->group_index) & firing_position_group) != 0 )
                {
                    float dz = firing_position->position.n[2] - test_point->n[2];
                    float dy = firing_position->position.n[1] - test_point->n[1];
                    float dx = firing_position->position.n[0] - test_point->n[0];
                    if ( dy * dy + (dx * dx + dz * dz) < 16.0f )
                    {
                        if ( use_bsp_path_test )
                        {
                            if ( path_3d_available(global_structure_bsp, test_point, 0.0f,
                                    (const collision_bsp_test_vector_result *)firing_position, nullptr, nullptr) )
                                return 1;
                        }
                        else
                        {
                            float distance_reference;
                            path_state_estimated_distance(&state, &firing_position->position,
                                    firing_position->surface_index, &distance_reference, nullptr, nullptr);
                            if ( distance_reference < 4.0f )
                                return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
