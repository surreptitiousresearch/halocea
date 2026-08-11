/* looping_sound_new_sound @ 0x83717AA0 — spawn a runtime sound instance for one track of a playing
 * looping sound (intro/loop/outro, per `type`). Bails (-1) if the definition has no usable
 * permutations, its class is disabled, the source is inaudible, or the runtime sound pool is full.
 * Otherwise it allocates a sound datum, copies the looping sound's source geometry into it, rolls a
 * random pitch within the definition's bounds, selects pitch range + first permutation, marks the
 * local-player flag, requests the permutation from the cache, bumps the looping sound's active
 * count, and returns the new sound index. */

#include <stdint.h>
#include <string.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/looping_sound_datum.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_definition.h"
#include "headers/sound_class_definition.h"
#include "headers/sound_source.h"
#include "headers/sound_permutation.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

#include "headers/sound_source.h"
extern int track_loop_track_sound(int looping_sound_index, const void *unused, sound_source *source);

extern float sound_definition_get_maximum_distance(int sound_definition_index);
extern int16_t source_audible(sound_source *source, float maximum_distance);
extern int datum_new(data_array *data);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern int16_t sound_definition_find_pitch_range_by_pitch(const sound_definition *sound, float pitch, int16_t current_range);
extern int16_t sound_definition_next_permutation(sound_definition *sound, int16_t pitch_range_index, int16_t looping_last_permutation_index);
extern uint8_t check_is_local_player_by_loop(int looping_sound_index);
extern uint8_t _sound_cache_sound_request(sound_permutation *sound, uint8_t block, uint8_t load, uint8_t reference);

int looping_sound_new_sound(int looping_sound_index, int definition_index, int16_t track_index, int16_t type)
{
    looping_sound_datum *looping = DATA_ARRAY_ELEMENT(looping_sound_data, looping_sound_datum, looping_sound_index);
    sound_definition *definition = TAG_GET(sound_definition, definition_index);
    float distance_scale = looping->source.scale;
    int usable;

    /* require at least one pitch range whose first range has permutations, and an enabled class */
    if ( !definition->pitch_ranges.count
      || !((sound_pitch_range *)definition->pitch_ranges.address)->permutations.count
      || sound_classes[definition->class_index].disabled )
        usable = 0;
    else
        usable = 1;

    if ( !usable )
        return -1;

    {
        float maximum_distance = sound_definition_get_maximum_distance(definition_index);
        int16_t audibility = source_audible(&looping->source, maximum_distance);
        int sound_index;
        sound_datum *new_datum;
        float pitch_modifier;
        int16_t pitch_range;
        unsigned char is_local_player;
        int new_definition_index;

        if ( audibility == -1 )
            return -1;

        sound_index = datum_new(sound_data);
        if ( sound_index == -1 )
            return -1;

        new_datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);
        new_datum->definition_index = definition_index;
        new_datum->playing_channel_index = -1;
        new_datum->listener_index = audibility;
        new_datum->flags = 0;

        pitch_modifier = real_seed_random_range(get_global_local_random_seed_address(),
                                                definition->pitch_lower_bound, definition->pitch_upper_bound);
        new_datum->pitch = pitch_modifier;
        new_datum->source_identifier = looping_sound_index;
        memcpy(&new_datum->source, &looping->source, 0x40u);
        new_datum->type = type;
        /* DEVIATION: was `extern int` + int-punned address store; track_loop_track_sound is the
         * in-corpus tracking callback (returns int; the track_proc field type says uint8_t —
         * width-compatible boolean result, cast bridges the return spelling). */
        new_datum->track_proc = (uint8_t (*)(int, const void *, sound_source *))track_loop_track_sound;
        new_datum->start_time = sound_manager_globals.render_time;
        new_datum->loop_track_index = track_index;
        new_datum->fade_stop_time = 0;
        new_datum->fade_start_time = 0;
        new_datum->next_definition_index = -1;

        /* DEVIATION: current_range is slot 2 = r5 (the float pitch at slot 1 eats r4); the binary
         * materialises li r5,-1 @0x83717C00, not the datum's flags word Hex-Rays attributed here. */
        pitch_range = sound_definition_find_pitch_range_by_pitch(
                          definition,
                          (((definition->scale_upper_bound.pitch - definition->scale_lower_bound.pitch)
                                          * distance_scale)
                                  + definition->scale_lower_bound.pitch) * pitch_modifier,
                          -1);
        new_datum->pitch_range_index = pitch_range;
        new_datum->permutation_index = sound_definition_next_permutation(definition, pitch_range, -1);

        is_local_player = check_is_local_player_by_loop(looping->loop_identifier);
        new_definition_index = new_datum->definition_index;
        new_datum->is_local_player = is_local_player;

        {
            sound_definition *new_def = TAG_GET(sound_definition, new_definition_index);
            sound_permutation *permutation =
                (sound_permutation *)((sound_pitch_range *)new_def->pitch_ranges.address
                                      + new_datum->pitch_range_index)->permutations.address
                + new_datum->permutation_index;
            _sound_cache_sound_request(permutation, 0, 1u, 0);
        }

        ++looping->component_sound_count;
        return sound_index;
    }
}
