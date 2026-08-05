/* device_preprocess_node_orientations @0x837B4C88 — apply a device's (machine/door/etc.) animation-graph
 * overlays onto `node_orientations`. Looks up the device tag's animation graph, then its first
 * "device_animations" block entry (a nested index-list keyed by two selector slots at record+84/+88):
 * selector 0 drives the device's own open/close-position overlay (blended between "position" and
 * "1 - position" per a reversed-direction flag on the device tag, at a frame chosen by the definition's
 * bit0/bit1 flags — bit0 selects frame_count vs. frame_count-1, bit1 selects an exact frame
 * (overlay_animation_apply) vs. a continuous blend (overlay_animation_apply_continuous)); selector 1, if
 * present, applies a second continuous overlay driven by a separate device object scale field.
 *
 * DEVIATION: the decompiler badly mis-modeled this function (visible from the bogus
 * `HIDWORD(v5) = &XBM__CaptureCompletionSignalMarker` line, which doesn't exist in the disassembly at all)
 * and, worse, displayed the wrong variable for `node_orientations` in the 2nd and 3rd overlay calls
 * (`(real_orientation *)v4` — v4 by that point holds a reused TAG_INSTANCE scratch address, not the
 * parameter). Disasm confirms all three calls actually pass r29, which is `node_orientations` saved
 * unchanged at function entry (`mr r29, r4`) — the IDA disassembly itself annotates that register
 * `# node_orientations` at each call site. Rebuilt entirely from disasm
 * (0x837B4C88-0x837B4E30). The per-device selector record is animation_graph_device_animations
 * (its +84/+88 selector fields are the "animations" tag_block). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/device_datum.h"
#include "headers/device_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_device_animations.h"
#include "headers/animation.h"
#include "headers/real_orientation.h"
#include "headers/device_datum_flags.h"
#include "headers/device_position_flags.h"
#include "headers/blam_data_globals.h"


extern void overlay_animation_apply(const animation *animation, int16_t frame_index, real_orientation *node_orientations);
extern void overlay_animation_apply_continuous(const animation *animation, float real_frame_index, real_orientation *node_orientations);

void device_preprocess_node_orientations(int device_index, real_orientation *node_orientations)
{
    device_datum *device = (device_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, device_index)->datum;
    device_definition *definition = TAG_GET(device_definition, device->definition_index);
    int animation_graph_tag_index = definition->object.animation_graph.index;
    animation_graph *graph = TAG_GET(animation_graph, animation_graph_tag_index);

    if ( graph->device_animations.count == 0 || graph->device_animations.address == 0 )
        return;

    animation_graph_device_animations *selector_record =
            (animation_graph_device_animations *)graph->device_animations.address;

    /* selector 0: the device's own open/close-position overlay.
     * selector_record->animations is a tag_block of __int16 animation indices. */
    int16_t position_animation_index = selector_record->animations.count > 0
            ? *(int16_t *)selector_record->animations.address
            : -1;

    if ( position_animation_index != -1 )
    {
        const animation *position_animation =
                &((animation *)graph->animations.address)[position_animation_index];

        unsigned char reversed = device->device.flags & (1u << _device_position_reversed_bit);
        float position = device->device.position;
        float scaled_position = reversed ? (1.0f - position) : position;

        unsigned int flags = definition->device.flags;
        int frame_count = position_animation->frame_count;
        int reference_frame = (flags & (1u << _device_position_loops_bit)) ? frame_count : frame_count - 1;
        float real_frame_index = (float)reference_frame * scaled_position;

        if ( flags & (1u << _device_position_animation_not_interpolated_bit) )
            overlay_animation_apply(position_animation, (int16_t)(int)real_frame_index, node_orientations);
        else
            overlay_animation_apply_continuous(position_animation, real_frame_index, node_orientations);
    }

    /* selector 1: an optional secondary continuous overlay */
    int16_t secondary_animation_index = selector_record->animations.count > 1
            ? ((int16_t *)selector_record->animations.address)[1]
            : -1;

    if ( secondary_animation_index != -1 )
    {
        const animation *secondary_animation =
                &((animation *)graph->animations.address)[secondary_animation_index];
        float secondary_scale = device->device.power; /* device power drives the secondary overlay scale */
        float real_frame_index = (float)secondary_animation->frame_count * secondary_scale;

        overlay_animation_apply_continuous(secondary_animation, real_frame_index, node_orientations);
    }
}
