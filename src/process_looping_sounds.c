/* process_looping_sounds @ 0x83719FA0 — per-frame service of every active looping sound. A looping
 * sound whose flip-flop byte matches the global flip-flop is "live": each of its detail layers whose
 * next-spawn time has arrived (and whose surface/underwater flags match the listener) fires a one-shot
 * impulse via sound_new_impulse (tracked by track_loop_impulse_sound with a random position offset),
 * then reschedules itself a random period later. A looping sound whose flip-flop no longer matches is
 * being torn down: its music (class 32) tracks' cached permutations are evicted and the datum is freed. */

#include "headers/sound_manager_globals.h"
#include "headers/looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/looping_sound_track.h"
#include "headers/looping_sound_detail.h"
#include "headers/loop_impulse_sound_tracking_data.h"
#include "headers/sound_definition.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_source.h"
#include "headers/sound_permutation.h"
#include "headers/sound_class.h"
#include "headers/detail_dont_play_flags.h"
#include "headers/looping_sound_refresh_state.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern uint8_t sound_cache_software_locks(const sound_permutation *sound);
extern void sound_cache_sound_delete(sound_permutation *sound);
extern void datum_delete(data_array *data, int index);
extern void detail_sound_random_offset(const looping_sound_detail *detail_definition, real_vector3d *offset);
extern int track_loop_impulse_sound(int looping_sound_index, const loop_impulse_sound_tracking_data *track_data, sound_source *source);
extern int sound_new_impulse(int definition_index, sound_source *source, int source_identifier, uint8_t (*track_proc)(int, const void *, sound_source *), const void *track_data, int16_t track_data_size, uint8_t is_local_player);
extern uint32_t *get_global_local_random_seed_address(void);
#include <stdint.h>
#include "headers/spatialization_mode.h"
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

/* Evict the cached permutations of a music (class 32) track sound. Returns 0 (caller should stop
 * walking tracks) when the referenced sound exists but is not music; 1 to continue. */
static int release_track_music_cache(int sound_tag_index)
{
    sound_definition *sound_def;
    int pitch_range_count;
    int pr;

    if ( sound_tag_index == -1 )
        return 1;

    sound_def = TAG_GET(sound_definition, sound_tag_index);
    if ( sound_def->class_index != _sound_class_music )
        return 0;

    pitch_range_count = sound_def->pitch_ranges.count;
    for ( pr = 0; pr < pitch_range_count; ++pr )
    {
        sound_pitch_range *pitch_range = (sound_pitch_range *)sound_def->pitch_ranges.address + pr;
        int permutation_count = pitch_range->permutations.count;
        int p;
        for ( p = 0; p < permutation_count; ++p )
        {
            sound_permutation *permutation = (sound_permutation *)pitch_range->permutations.address + p;
            if ( permutation->cache_block_index != -1 && !sound_cache_software_locks(permutation) )
                sound_cache_sound_delete(permutation);
        }
    }
    return 1;
}

void process_looping_sounds(void)
{
    int i = data_next_index(looping_sound_data, -1);

    while ( i != -1 )
    {
        looping_sound_datum *loop_datum = DATA_ARRAY_ELEMENT(looping_sound_data, looping_sound_datum, i);
        looping_sound_definition *def =
            TAG_GET(looping_sound_definition, loop_datum->definition_index);

        if ( loop_datum->flip_flop == sound_manager_globals.flip_flop )
        {
            /* live: fire any due detail one-shots */
            if ( loop_datum->state != _looping_sound_refresh_stop && def->details.count > 0 )
            {
                int detail_index;
                for ( detail_index = 0; detail_index < def->details.count;
                      detail_index = (int16_t)(detail_index + 1) )
                {
                    int *next_time = &loop_datum->details[detail_index].next_play_time;
                    looping_sound_detail *detail =
                        (looping_sound_detail *)def->details.address + detail_index;

                    if ( *next_time < sound_manager_globals.render_time )
                    {
                        int sound_index = detail->sound.index;
                        if ( sound_index != -1 )
                        {
                            int flags = detail->flags;
                            float scale = loop_datum->source.scale;
                            sound_definition *sound_def = TAG_GET(sound_definition, sound_index);

                            if ( ((flags & (1u << _detail_dont_play_with_alternate_bit)) == 0 || !loop_datum->alternate)
                              && ((flags & (1u << _detail_dont_play_without_alternate_bit)) == 0 || loop_datum->alternate) )
                            {
                                int spatialization = (uint16_t)loop_datum->source.spatialization_mode;
                                loop_impulse_sound_tracking_data tracking;
                                sound_source source;

                                source.gain = detail->gain;
                                source.scale = scale;
                                source.spatialization_mode = (spatialization == _sound_spatialization_mode_none) + 1;
                                detail_sound_random_offset(detail, &tracking.position_offset);
                                track_loop_impulse_sound(i, &tracking, &source);
                                /* register a specific callback into the generic void*-data callback slot: funcptr cast is faithful */
                                sound_new_impulse(detail->sound.index, &source, i,
                                                  (unsigned char (*)(int, const void *, sound_source *))track_loop_impulse_sound,
                                                  &tracking, 12, 0);
                            }

                            /* reschedule a random period later (scaled by the definition's period
                             * bounds and the loop scale) */
                            {
                                float period_lower = def->scale_lower_bound.detail_period;
                                float period_upper = def->scale_upper_bound.detail_period;
                                float period_factor = (period_upper - period_lower) * scale + period_lower;
                                float random_period = real_seed_random_range(
                                    get_global_local_random_seed_address(),
                                    detail->period_lower_bound, detail->period_upper_bound);
                                /* Deviation: the decompiler packed render_time and the sound's max
                                 * play-time into a mangled __int128; the evident intent is
                                 * render_time + play_time + period*1000. */
                                *next_time = (int)(period_factor * random_period * 1000.0f
                                                   + (float)sound_def->runtime_maximum_play_time
                                                   + (float)sound_manager_globals.render_time);
                            }
                        }
                    }
                }
            }
        }
        else
        {
            /* torn down: evict music-track caches, then free the looping-sound datum */
            if ( def->tracks.count > 0 )
            {
                int track_index;
                for ( track_index = 0; track_index < def->tracks.count;
                      track_index = (int16_t)(track_index + 1) )
                {
                    looping_sound_track *track = (looping_sound_track *)def->tracks.address + track_index;
                    if ( !release_track_music_cache(track->start_sound.index) )
                        break;
                    if ( !release_track_music_cache(track->stop_sound.index) )
                        break;
                    if ( !release_track_music_cache(track->alternate_stop_sound.index) )
                        break;
                    if ( !release_track_music_cache(track->loop_sound.index) )
                        break;
                    if ( !release_track_music_cache(track->alternate_loop_sound.index) )
                        break;
                }
            }
            datum_delete(looping_sound_data, i);
        }

        i = data_next_index(looping_sound_data, i);
    }
}
