/* scripted_looping_sound_set_scale @0x83713CE8 — set the [0,1] intensity scale of a running scripted looping
 * sound. The looping-sound datum is found via the sound tag's active looping-sound index
 * (looping_sound_definition.runtime_scripting_sound_index); the scale is stored at
 * game_looping_sound_datum.scale (+0x8). */

#include "headers/global_tag_instances.h"
#include "headers/looping_sound_definition.h"
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/blam_data_globals.h"


void scripted_looping_sound_set_scale(int definition_index, float scale)
{
    if ( definition_index == -1 )
        return;
    int looping_index = TAG_GET(looping_sound_definition, definition_index)->runtime_scripting_sound_index;   /* +28 */
    if ( looping_index == -1 )
        return;

    float clamped = scale;
    if ( scale < 0.0f )
        clamped = 0.0f;
    else if ( scale > 1.0f )
        clamped = 1.0f;

    /* game_looping_sound_data is really a data_array* (DB); blam_data_globals mistypes it — cast. */
    DATUM_GET((data_array *)game_looping_sound_data, game_looping_sound_datum, looping_index)->scale = clamped;
}
