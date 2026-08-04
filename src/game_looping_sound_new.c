/* game_looping_sound_new @0x83713988 — register a new looping sound. If attached to an object, the
 * named marker is resolved and its position/forward cached so the looping sound tracks the object node.
 * Returns the new looping-sound datum index, or -1 if the definition is invalid or the marker is missing.
 *
 * Fields resolved to the DB game_looping_sound_datum struct (52 bytes): flags, definition_index,
 * object_index, last-serviced frame, state, function index, node index, position @28, forward @40. */

/* blam_data_globals.h first: supplies wchar_t before dependent headers are parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/data_array.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/game_looping_sound_state.h"
#include "headers/object_marker.h"


extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int datum_new(data_array *data);

int game_looping_sound_new(int object_index, int definition_index, const char *marker_name, int16_t function_index)
{
    object_marker marker;
    int index;
    game_looping_sound_datum *sound;

    if ( definition_index == -1
      || (object_index != -1 && !object_get_marker_by_name(object_index, marker_name, &marker, 1)) )
        return -1;

    index = datum_new(game_looping_sound_data);
    if ( index != -1 )
    {
        sound = DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, index);
        sound->object_index = object_index;
        sound->definition_index = definition_index;
        sound->state = _game_looping_sound_inactive;
        sound->flags = 0;
        sound->attachment.function_index = function_index;
        sound->last_audible_frame_index = -1;
        if ( object_index != -1 )
        {
            sound->attachment.node_index = marker.node_index;
            sound->attachment.position.n[0] = marker.node_matrix.n[3][0];
            sound->attachment.position.n[1] = marker.node_matrix.n[3][1];
            sound->attachment.position.n[2] = marker.node_matrix.n[3][2];
            sound->attachment.forward.n[0] = marker.node_matrix.n[0][0];
            sound->attachment.forward.n[1] = marker.node_matrix.n[0][1];
            sound->attachment.forward.n[2] = marker.node_matrix.n[0][2];
        }
    }
    return index;
}
