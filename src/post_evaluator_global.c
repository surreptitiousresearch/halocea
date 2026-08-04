/* post_evaluator_global @0x837EF110 — "global" post-evaluator for firing-position selection: only acts
 * when the context is flying (3D pathing relevant). With no position given, just bounds the context by
 * 15.0. With a position, checks whether 3D pathing is actually needed for the actor
 * (actor_path_3d_available) and, if so, whether a straight-line 3D path from the actor's own position
 * (actor +300, per the salted-offset convention) to the firing position clears collision
 * (path_3d_available, reusing firing_position_definition's leading real_point3d as both the
 * destination_point and the type-punned collision_bsp_test_vector_result per path_3d_available.c's
 * established convention). A clear path adds a flat 15.0 bonus; a blocked one rejects the position
 * (invalidating it unless rejected positions are allowed). Returns the position's validity (or 1 if just
 * bounding the context). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/firing_position.h"
#include "headers/structure_bsp.h"
#include "headers/collision_bsp_test_vector_result.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/structure_bsp.h"
#include "headers/real_point3d.h"
extern uint8_t actor_path_3d_available(uint16_t actor_index, const real_point3d *destination_point, float *avoidance_distance_reference);
extern uint8_t path_3d_available(structure_bsp *structure_bsp, const real_point3d *start_point, float avoidance_distance, const collision_bsp_test_vector_result *destination_reference, uint8_t *path_available_out, float *hit_result_out);

unsigned __int8 post_evaluator_global(int actor_index, firing_position_evaluation_context *evaluation_context,
        firing_position *firing_position)
{
    if ( evaluation_context->flying )
    {
        if ( !firing_position )
        {
            evaluation_context->post_evaluation_bound = evaluation_context->post_evaluation_bound + 15.0f;
            return 1;
        }

        actor_datum *actor = &((actor_datum *)actor_data->data)[actor_index];
        float avoidance_distance = 0.0f;

        /* recovered: (const real_point3d *)definition -> &definition->position (leading member @0x0) */
        unsigned __int8 path_available = actor_path_3d_available(actor_index,
            &firing_position->definition->position, &avoidance_distance);

        /* actor +300 = actor_datum.input.position.body_position (0x120 + 0xC) */
        if ( path_available )
            path_available = path_3d_available(global_structure_bsp, &actor->input.position.body_position,
                avoidance_distance, (const collision_bsp_test_vector_result *)firing_position->definition, 0, 0);

        if ( path_available )
        {
            firing_position->evaluation = firing_position->evaluation + 15.0f;
        }
        else
        {
            firing_position->rejected = 1;
            if ( !evaluation_context->allow_rejected_positions )
                firing_position->valid = 0;
        }
    }

    return firing_position ? firing_position->valid : 1;
}
