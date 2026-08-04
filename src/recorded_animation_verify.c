/* recorded_animation_verify @0x83712188 — debug/validation pass over one recorded-animation definition:
 * initialize the recording's playback codec (selected by its 1-based version, like
 * recorded_animation_play_internal's playback_codec[version - 1]) over the raw event stream, then apply
 * every event tick until the codec reports the stream exhausted.
 *
 * DEVIATION: the decompiler rendered the codec-table lookup as
 * `(char *)&playback_codec[-1] + __ROL4__(version, 2)`, which for the small version values in play is
 * exactly playback_codec[version - 1]; written in that direct form to match the established sibling. */

#include "headers/recorded_animation_definition.h"
#include "headers/animation_playback.h"
#include "headers/animation_playback_controller.h"
#include "headers/unit_control_data.h"
#include "headers/blam_data_globals.h"


void recorded_animation_verify(const recorded_animation_definition *recording)
{
    const char *event_stream = (const char *)recording->animation_data.address;
    int tick = 0;
    /* recovered: the raw controller[24]/control[72] byte buffers are oversized decompiler stack slots
     * for the typed playback structs the codec interface actually takes */
    animation_playback_controller controller;
    unit_control_data control;

    playback_codec[recording->version - 1]->initialize_event_stream(
        &controller,
        &control,
        &event_stream,
        recording->unit_control_data_version);

    unsigned __int8 more;
    do
    {
        more = playback_codec[recording->version - 1]->apply_event_stream(
                   &controller,
                   &control,
                   &tick,
                   &event_stream);
        ++tick;
    }
    while ( more );
}
