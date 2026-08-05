/* vehicle_preprocess_node_orientations @0x8375D388 — drive a vehicle's animated node orientations from its
 * current motion state before the render/physics pass reads them. Resolves the vehicle's unit definition and
 * its model_animation_graph, then applies:
 *   - the aiming screen (steering direction) for the primary aiming animation,
 *   - four fixed overlay animations mapped from motion quantities (a velocity/basis triple product -> yaw
 *     overlay 0, a steering value -> overlay 1, a forward-dot-velocity -> overlay 2, a scalar -> overlay 3),
 *   - and one overlay per graph-defined weighted node, scaled by that node's 0..255 weight byte.
 * Each overlay's frame index is the animation's (frame_count - 1) times the 0..1 factor (frame_count for
 * overlay 3), matching the shipped code.
 *
 * The vehicle object datum and tag definitions are read at raw byte offsets (char *), per project convention.
 *
 * DEVIATIONS (disasm-resolved, 0x8375D388-0x8375D7E0):
 *  - node_orientations (r4) is saved to r27 at entry and passed to every aiming_screen_apply /
 *    overlay_animation_apply_continuous call. The decompiler reused r4 for the tag-record pointer and so
 *    mis-rendered the orientation argument as `v3` / `*(real_orientation **)v2`; the real argument is
 *    node_orientations throughout.
 *  - aiming_screen_apply is an FPR-shadow call: its two float args (direction, elevation) sit in f1/f2 and
 *    shadow r5/r6, so node_orientations is passed in r7.
 *  - The HIDWORD/LODWORD __int64 splits around each frame index are the fcfid int->float conversion of a
 *    32-bit (frame_count - 1); reconstructed as (float)(frame_count - 1) * factor. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/animation.h"
#include "headers/animation_aiming_screen_bounds.h"
#include "headers/real_orientation.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_vehicle_animations.h"
#include "headers/animation_graph_vehicle_suspension_animation.h"
#include "headers/blam_data_globals.h"


#include "headers/animation_aiming_screen_bounds.h"
extern void aiming_screen_apply(const animation *animation, const animation_aiming_screen_bounds *aiming_screen, float direction, float elevation, real_orientation *node_orientations);
extern void overlay_animation_apply_continuous(const animation *animation, float real_frame_index, real_orientation *node_orientations);
extern float fabsf(float x);

void vehicle_preprocess_node_orientations(int vehicle_index, real_orientation *node_orientations)
{
    vehicle_datum *vehicle_object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    vehicle_definition *definition = TAG_GET(vehicle_definition, vehicle_object->definition_index);

    int animation_graph_index = definition->object.animation_graph.index;
    if ( animation_graph_index == -1 )
        return;

    animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
    if ( !graph->vehicle_animations.count )
        return;

    animation_graph_vehicle_animations *unit_block = (animation_graph_vehicle_animations *)graph->vehicle_animations.address;
    if ( !unit_block )
        return;

    char *animations_data = (char *)graph->animations.address;
    int screen_count = unit_block->animations.count;
    int16_t *aiming_indices = (int16_t *)unit_block->animations.address;

    /* aiming screen: primary aiming animation driven by the steering direction */
    int16_t aiming_index = screen_count <= 0 ? -1 : aiming_indices[0];
    if ( aiming_index != -1 )
        aiming_screen_apply((const animation *)(180 * aiming_index + animations_data),
                &unit_block->steering_screen_bounds, vehicle_object->vehicle.turn, 0.0f,
                node_orientations);

    /* overlay 0: yaw from velocity . (basis triple product), mapped to 0..1 */
    int16_t overlay0_index = screen_count <= 1 ? -1 : aiming_indices[1];
    if ( overlay0_index != -1 )
    {
        const animation *overlay0 = (const animation *)(180 * overlay0_index + animations_data);
        float yaw = (((((vehicle_object->object.translational_velocity.n[0]
                                        * ((vehicle_object->object.forward.n[2] * vehicle_object->object.up.n[1])
                                                - (vehicle_object->object.forward.n[1] * vehicle_object->object.up.n[2])))
                                + (vehicle_object->object.translational_velocity.n[2]
                                        * ((vehicle_object->object.forward.n[1] * vehicle_object->object.up.n[0])
                                                - (vehicle_object->object.forward.n[0] * vehicle_object->object.up.n[1]))))
                        + (vehicle_object->object.translational_velocity.n[1]
                                * ((vehicle_object->object.forward.n[0] * vehicle_object->object.up.n[2])
                                        - (vehicle_object->object.forward.n[2] * vehicle_object->object.up.n[0]))))
                / definition->vehicle.speed.maximum_forward_speed)
                + (float)1.0)
                * (float)0.5; /* removed stray extra ')': outer cast group already closed after +1.0 */
        if ( yaw >= 0.0f )
            yaw = yaw > 1.0f ? 1.0f : yaw;
        else
            yaw = 0.0f;
        overlay_animation_apply_continuous(overlay0, (float)(overlay0->frame_count - 1) * yaw, node_orientations);
    }

    /* overlay 1: signed steering value, split about zero by the definition's left/right ranges */
    int16_t overlay1_index = screen_count <= 2 ? -1 : aiming_indices[2];
    if ( overlay1_index != -1 )
    {
        const animation *overlay1 = (const animation *)(180 * overlay1_index + animations_data);
        float steer;
        if ( vehicle_object->vehicle.speed >= 0.0f )
            steer = (((vehicle_object->vehicle.speed / definition->vehicle.speed.maximum_forward_speed) + (float)1.0)
                    * (float)0.5);
        else
            steer = -(((vehicle_object->vehicle.speed / definition->vehicle.speed.maximum_reverse_speed)
                            * (float)0.5)
                    - (float)0.5);
        overlay_animation_apply_continuous(overlay1, (float)(overlay1->frame_count - 1) * steer, node_orientations);
    }

    /* overlay 2: forward-dot-velocity, clamped, normalized by the yaw range magnitude */
    int16_t overlay2_index = screen_count <= 3 ? -1 : aiming_indices[3];
    if ( overlay2_index != -1 )
    {
        const animation *overlay2 = (const animation *)(180 * overlay2_index + animations_data);
        float pitch = ((vehicle_object->object.forward.n[0] * vehicle_object->object.translational_velocity.n[0])
                + ((vehicle_object->object.forward.n[2] * vehicle_object->object.translational_velocity.n[2])
                        + (vehicle_object->object.forward.n[1] * vehicle_object->object.translational_velocity.n[1])));
        if ( pitch >= 0.0f )
            pitch = pitch > 1.0f ? 1.0f : pitch;
        else
            pitch = 0.0f;
        float normalized = (pitch / fabsf(definition->vehicle.speed.maximum_forward_speed));
        if ( normalized >= 0.0f )
            normalized = normalized > 1.0f ? 1.0f : normalized;
        else
            normalized = 0.0f;
        overlay_animation_apply_continuous(overlay2, (float)(overlay2->frame_count - 1) * normalized, node_orientations);
    }

    /* overlay 3: scalar ratio; uses frame_count (not frame_count - 1) */
    int16_t overlay3_index = screen_count <= 5 ? -1 : aiming_indices[5];
    if ( overlay3_index != -1 )
    {
        const animation *overlay3 = (const animation *)(180 * overlay3_index + animations_data);
        float ratio;
        if ( definition->vehicle.wheel_circumference <= 0.0f )
            ratio = 0.0f;
        else
            ratio = (vehicle_object->vehicle.wheel / definition->vehicle.wheel_circumference);
        overlay_animation_apply_continuous(overlay3, (float)overlay3->frame_count * ratio, node_orientations);
    }

    /* one overlay per graph-defined weighted node, scaled by its 0..255 weight byte */
    int weighted_node_count = unit_block->suspension_animations.count;
    if ( weighted_node_count > 0 )
    {
        animation_graph_vehicle_suspension_animation *weighted_nodes_data =
                (animation_graph_vehicle_suspension_animation *)unit_block->suspension_animations.address;
        for ( int i = 0; i < weighted_node_count; i = (int16_t)(i + 1) )
        {
            int16_t node_animation_index = weighted_nodes_data[i].animation_index;
            if ( node_animation_index != -1 )
            {
                const animation *node_animation = (const animation *)(180 * node_animation_index + animations_data);
                uint8_t weight_byte = vehicle_object->vehicle.suspension[i];
                float weight = weight_byte == 255 ? 1.0f : ((float)weight_byte * (float)0.0039215689);
                overlay_animation_apply_continuous(node_animation,
                        (float)(node_animation->frame_count - 1) * weight, node_orientations);
            }
        }
    }
}
