/* recorded_animation_play_internal @0x837123E0 — begin playing a recorded animation on a unit.
 *
 * Validates the unit and animation index, then acquires (or allocates) the unit's controlling
 * animation_thread, seeds it from the recorded-animation record (64-byte stride in the scenario's
 * recorded_animations block: tick count @+36, event-stream pointer @+56, codec version @+32, codec init
 * parameter @+34), and initializes the codec's event stream over the thread's control data. The unit is
 * then switched into actively-controlled/possessed mode. extra_flags (delete / hover behavior) are OR'd
 * into the thread's flags. Returns 1 on success, 0 if anything failed. */

#include <stdint.h>
#include "headers/recorded_animation_definition.h"
#include "headers/animation_thread.h"
#include "headers/scenario.h"
#include "headers/data_array.h"

#include "headers/animation_playback.h"
#include "headers/recording_thread_flags.h"
#include "headers/blam_data_globals.h"


extern int player_index_from_unit_index(int unit_index);
extern void * get_controlling_thread(int unit_index, int *thread_index_reference);
extern uint8_t recorded_animation_controlling_unit(int unit_index);
extern int datum_new(data_array *data);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern uint8_t unit_controllable(int unit_index);
extern void unit_set_controllable(int unit_index, uint8_t controllable);
extern void unit_set_possessed(int unit_index, uint8_t possessed);
extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);

uint8_t recorded_animation_play_internal(int unit_index, int16_t animation_index, uint16_t extra_flags)
{
    if ( unit_index == -1 )
        return 0;
    if ( animation_index == -1 )
        return 0;
    if ( animation_index >= global_scenario->recorded_animations.count )
        return 0;

    player_index_from_unit_index(unit_index);   /* result unused; called for its validation side effect */

    int controlling_thread_index;
    animation_thread *thread = get_controlling_thread(unit_index, &controlling_thread_index);

    const recorded_animation_definition *record = (const recorded_animation_definition *)global_scenario->recorded_animations.address + animation_index;

    if ( recorded_animation_controlling_unit(unit_index) )
        return 0;

    if ( !thread )
    {
        int new_index = datum_new(animation_threads);
        if ( new_index == -1 )
            return 0;
        thread = DATA_ARRAY_ELEMENT(animation_threads, animation_thread, new_index);
        if ( !thread )
            return 0;
    }

    thread->unit_index = unit_index;
    thread->relative_ticks = 0;
    thread->ticks_left = record->ticks;
    thread->event_stream = (const char *)record->animation_data.address;
    __int16 codec_version = record->version - 1;
    thread->version = codec_version;
    thread->flags &= ~(1u << _recording_thread_finished_bit);

    /* recovered: controller/animation_state are now typed members -> casts no longer needed */
    playback_codec[codec_version]->initialize_event_stream(
        &thread->animation_state,
        &thread->controller,
        &thread->event_stream,
        record->unit_control_data_version);

    unit_set_actively_controlled(unit_index, 1u);
    if ( unit_controllable(unit_index) )
        thread->flags |= (1u << _recording_thread_unit_was_controlled_bit);
    else
        thread->flags &= ~(1u << _recording_thread_unit_was_controlled_bit);

    unit_set_controllable(unit_index, 0);
    unit_set_possessed(unit_index, 1u);
    object_set_automatic_deactivation(unit_index, 0);
    thread->flags |= extra_flags;
    return 1;
}
