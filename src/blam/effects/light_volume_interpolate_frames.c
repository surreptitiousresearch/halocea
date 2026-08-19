/* light_volume_interpolate_frames @0x8380AB18 — produce the animated light-volume frame for an object this
 * tick. With zero or one keyframe it returns the frame array directly; otherwise it samples the object
 * function named by frame_animation_source and blends the keyframe fields by that fraction into the shared
 * frame_storage, returning it (falling back to the raw frames when the function value is unavailable).
 *
 * DEVIATION: none functionally, but note the blend `frame->x*(1-t) + frame->x*t` reads the SAME keyframe (0)
 * for both endpoints (disasm 0x8380AB70-0x8380AB94 loads one value into both operands), so it collapses to
 * frame[0].x regardless of t — the interpolation is a no-op in this build (an original-code quirk, reproduced
 * faithfully rather than "corrected" to a second keyframe). */

#include <stdint.h>
#include "headers/light_volume_definition.h"
#include "headers/light_volume_frame.h"
#include "headers/blam_data_globals.h"

extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);

light_volume_frame * light_volume_interpolate_frames(const light_volume_definition *definition, int object_index)
{
    void *address = definition->frames.address;
    if ( definition->frames.count <= 1 )
        return (light_volume_frame *)address;

    const light_volume_frame *frame = (const light_volume_frame *)definition->frames.address;

    float function_value[2];
    if ( !object_get_function_value(object_index, definition->frame_animation_source - 1, function_value) )
        return (light_volume_frame *)address;

    float t = function_value[0];
    float inverse_t = (float)1.0 - t;

    frame_storage.offset_from_marker = (frame->offset_from_marker * inverse_t) + (frame->offset_from_marker * t);
    frame_storage.offset_exponent = (frame->offset_exponent * inverse_t) + (frame->offset_exponent * t);
    frame_storage.length = (frame->length * inverse_t) + (frame->length * t);
    frame_storage.radius_hither = (frame->radius_hither * inverse_t) + (frame->radius_hither * t);
    frame_storage.radius_yon = (frame->radius_yon * inverse_t) + (frame->radius_yon * t);
    frame_storage.radius_exponent = (frame->radius_exponent * inverse_t) + (frame->radius_exponent * t);
    frame_storage.color_hither.n[0] = (frame->color_hither.n[0] * inverse_t) + (frame->color_hither.n[0] * t);
    frame_storage.color_hither.n[1] = (frame->color_hither.n[1] * inverse_t) + (frame->color_hither.n[1] * t);
    frame_storage.color_hither.n[2] = (frame->color_hither.n[2] * inverse_t) + (frame->color_hither.n[2] * t);
    frame_storage.color_hither.n[3] = (frame->color_hither.n[3] * inverse_t) + (frame->color_hither.n[3] * t);
    frame_storage.color_yon.n[0] = (frame->color_yon.n[0] * inverse_t) + (frame->color_yon.n[0] * t);
    frame_storage.color_yon.n[1] = (frame->color_yon.n[1] * inverse_t) + (frame->color_yon.n[1] * t);
    frame_storage.color_yon.n[2] = (frame->color_yon.n[2] * inverse_t) + (frame->color_yon.n[2] * t);
    frame_storage.color_yon.n[3] = (frame->color_yon.n[3] * inverse_t) + (frame->color_yon.n[3] * t);
    frame_storage.color_exponent = (frame->color_exponent * inverse_t) + (frame->color_exponent * t);
    frame_storage.brightness_exponent = (frame->brightness_exponent * inverse_t) + (frame->brightness_exponent * t);
    return &frame_storage;
}
