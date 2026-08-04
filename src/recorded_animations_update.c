/* recorded_animations_update @ 0x83712008 — advance every active recorded-animation thread (scripted
 * unit animations / cutscene playback). For each thread whose target is still a valid unit:
 *   - if the thread is not yet finished (flags bit0 clear): decrement its remaining-frame counter, run
 *     the thread's codec over the event stream to drive the unit_control_data, push that control to the
 *     unit, and set the finished bit when the codec reports the stream is exhausted (result == 0).
 *   - if the thread is finished (flags bit0 set): hand the unit back to normal control
 *     (controllable per flags bit2, not possessed, not actively controlled, auto-deactivation on),
 *     optionally destroy it (bit3) or make its vehicle hover (bit4), then delete the thread.
 *
 * Recorded-animation datum (word-indexed via i): +4 (int) unit object index; +8 (word) remaining
 * frames; +10 (word) flags; +12 (int) event-stream cursor; +16 (char*) event-stream data; +20
 * unit_control_data; +84 animation_playback_controller; +96 (word) codec index.
 *
 * Deviation: the decompiler computed the finished flag via `_cntlzw(result) >> 5` — the branchless
 * "result == 0" test; restored to an explicit comparison. The decompiler's stray return value (the last
 * data_iterator_next result) is discarded; this is a void function. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/animation_playback_codec.h"
#include "headers/animation_thread.h"
#include "headers/object_type.h"
#include "headers/recording_thread_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
#include "headers/unit_control_data.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags); /* deviation: _DWORD -> void* (undeclared MSVC type) */
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void unit_set_controllable(int unit_index, uint8_t controllable);
extern void unit_set_possessed(int unit_index, uint8_t possessed);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern void object_set_automatic_deactivation(int object_index, uint8_t automatic_deactivation);
extern void hs_object_destroy(int object_index);
extern void vehicle_hover(int vehicle_index, uint8_t hover_flag);
extern void datum_delete(data_array *data, int index);

void recorded_animations_update(void)
{
    data_iterator iter;
    animation_thread *thread;

    data_iterator_new(&iter, animation_threads);
    for ( thread = data_iterator_next(&iter); thread;
          thread = data_iterator_next(&iter) )
    {
        int unit_index = thread->unit_index;
        __int16 flags;

        if ( !object_try_and_get_and_verify_type(unit_index, object_mask_unit) )
            continue;

        flags = thread->flags;
        if ( (flags & (1u << _recording_thread_finished_bit)) == 0 )
        {
            int codec_index = thread->version;   /* version doubles as the playback-codec selector */
            int result;

            --thread->ticks_left;                /* one frame consumed */
            result = playback_codec[codec_index]->apply_event_stream(
                         &thread->animation_state,
                         &thread->controller,
                         &thread->relative_ticks,
                         &thread->event_stream);
            ++thread->relative_ticks;             /* advance frame counter */
            unit_control(thread->unit_index, &thread->controller, -1);

            if ( result == 0 )
                thread->flags |= (1u << _recording_thread_finished_bit);   /* stream exhausted: thread finished */
            else
                thread->flags &= ~(1u << _recording_thread_finished_bit);
        }
        else
        {
            unit_set_controllable(unit_index, (flags & (1u << _recording_thread_unit_was_controlled_bit)) != 0);
            unit_set_possessed(unit_index, 0);
            unit_set_actively_controlled(unit_index, 0);
            object_set_automatic_deactivation(unit_index, 1u);
            if ( (thread->flags & (1u << _recording_thread_delete_unit_on_complete_bit)) != 0 )
                hs_object_destroy(unit_index);
            if ( (thread->flags & (1u << _recording_thread_hover_vehicle_on_complete_bit)) != 0 )
                vehicle_hover(unit_index, 1u);
            datum_delete(animation_threads, iter.index);
        }
    }
}
