/* scripted_sound_new @0x837149B8 — play a one-shot scripted sound, either world-positioned (no source
 * object) or attached to a source object's "head" marker (falling back to the world origin/forward if the
 * marker is missing). Stops any previous scripted instance, records the predicted end time on the sound tag
 * (runtime_scripting_time @0x90), and stores the new playing-sound handle (runtime_scripting_sound_index @0x94).
 *
 * DEVIATION: the decompiler derives object_impulse_sound_new's final byte argument from a float field
 * (a misread of an uninitialized register); passed as 0 here. Object-marker matrix rows are accessed as
 * n[0] (forward) and n[3] (position). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/sound_definition.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

struct sound_source;

#include "headers/sound_source.h"
extern void sound_stop_impulse(uint16_t sound_index);
extern int game_time_get(void);
extern int sound_new_impulse(int definition_index, sound_source *source, int source_identifier, uint8_t (*track_proc)(int, const void *, sound_source *), const void *track_data, int16_t track_data_size, uint8_t is_local_player);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);
extern void object_type_notify_impulse_sound(int object_index, int sound_definition_index, int impulse_sound_index);

void scripted_sound_new(int definition_index, int source_object_index, float scale)
{
    if ( definition_index == -1 )
        return;

    sound_definition *tag = TAG_GET(sound_definition, definition_index);
    sound_stop_impulse(tag->runtime_scripting_sound_index);
    tag->runtime_scripting_time = game_time_get() + 30 * tag->runtime_maximum_play_time / 1000;

    if ( scale < 0.0f )
        scale = 0.0f;
    else if ( scale > 1.0f )
        scale = 1.0f;

    int sound_handle;
    if ( source_object_index == -1 )
    {
        object_marker source;
        source.node_matrix.scale = scale;
        source.node_matrix.___u1.n[0][0] = 1.0f;   /* n[][] lives in the ___u1 row union */
        source.node_index = 0;
        sound_handle = sound_new_impulse(definition_index, (struct sound_source *)&source, -1, nullptr, nullptr, 0, 0);
    }
    else
    {
        object_marker marker;
        real_point3d position;
        real_vector3d forward;
        int16_t node_index;
        if ( object_get_marker_by_name(source_object_index, "head", &marker, 1) )
        {
            node_index = marker.node_index;
            position.n[0] = marker.node_matrix.___u1.n[3][0];
            position.n[1] = marker.node_matrix.___u1.n[3][1];
            position.n[2] = marker.node_matrix.___u1.n[3][2];
            forward.n[0] = marker.node_matrix.___u1.n[0][0];
            forward.n[1] = marker.node_matrix.___u1.n[0][1];
            forward.n[2] = marker.node_matrix.___u1.n[0][2];
        }
        else
        {
            node_index = 0;
            position = *global_origin3d;
            forward = *global_forward3d;
        }
        sound_handle = object_impulse_sound_new(source_object_index, definition_index, node_index,
                                                &position, &forward, scale, 0);
        if ( sound_handle != -1 )
        {
            object_type_notify_impulse_sound(source_object_index, definition_index, sound_handle);
            tag->runtime_scripting_sound_index = sound_handle;
            return;
        }
    }
    tag->runtime_scripting_sound_index = sound_handle;
}
