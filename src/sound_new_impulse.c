/* sound_new_impulse @ 0x83718578 — start a one-shot ("impulse") sound from a sound tag. Performs
 * dialog-timing bookkeeping, format/playability and random skip-fraction gating, audibility and
 * promotion checks, then allocates a runtime sound instance, fills in its pitch/permutation/
 * spatialization state, requests the sample data from the cache, and schedules it (immediately or
 * after a distance-based delay). Returns the new sound instance index, or -1 if it didn't play. */

#include <stdint.h>
#include <string.h>
#include "headers/sound_definition.h"
#include "headers/sound_datum.h"
#include "headers/sound_pitch_range.h"
#include "headers/sound_permutation.h"
#include "headers/sound_source.h"
#include "headers/sound_manager_globals.h"
#include "headers/cinematic_globals.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/spatialization_mode.h"
#include "headers/sound_class.h"
#include "headers/sound_compression.h"
#include "headers/sound_encoding.h"
#include "headers/sound_flags.h"
#include "headers/sound_type.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern uint32_t *get_global_local_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);
extern float sound_definition_get_maximum_distance(int sound_definition_index);
extern uint8_t sound_definition_is_playable(int definition_index);
extern int16_t source_audible(sound_source *source, float maximum_distance);
extern int datum_new(data_array *data);
extern int16_t sound_definition_find_pitch_range_by_pitch(const sound_definition *sound, float pitch, int16_t current_range);
extern int16_t sound_definition_next_permutation(sound_definition *sound, int16_t pitch_range_index, int16_t looping_last_permutation_index);
extern int    _sound_cache_sound_request(sound_permutation *sound, unsigned char block, unsigned char load, unsigned char reference);


extern int16_t sound_definition_promote(int definition_index);
int sound_new_impulse(int definition_index, sound_source *source, int source_identifier,
                      uint8_t (*track_proc)(int, const void *, sound_source *),
                      const void *track_data, int16_t track_data_size, uint8_t is_local_player)
{
    float scale = source->scale;
    sound_definition *def = TAG_GET(sound_definition, definition_index);
    int result = -1;
    int16_t class_index = def->class_index;
    sound_datum *snd;   /* runtime sound datum */
    int sound_index;
    int16_t audible_index;
    int16_t promotion;  /* 0 = play, 1 = play promotion sound, 2 = drop */
    int distance_delay;

    /* scripted-dialog timing window */
    if ( class_index == _sound_class_scripted_dialog_to_player
      || class_index == _sound_class_scripted_dialog_to_other
      || class_index == _sound_class_scripted_dialog_force_unspatialized )
    {
        int now = game_time_get();
        int play_ms = 30 * def->runtime_maximum_play_time;
        int latest = now + play_ms / 1000 + 10;
        if ( latest > sound_manager_globals.game_time_when_no_scripted_dialog_will_be_playing )
            sound_manager_globals.game_time_when_no_scripted_dialog_will_be_playing = latest;
        if ( loud_dialog_hack )
            source->spatialization_mode = _sound_spatialization_mode_none;
    }
    if ( def->class_index == _sound_class_scripted_dialog_force_unspatialized )
        source->spatialization_mode = _sound_spatialization_mode_none;

    if ( !sound_manager_globals.initialized
      || !sound_manager_globals.active
      || sound_manager_globals.disable_from_commandline )
        return result;

    /* only none/xbox-adpcm/ogg compression, mono unless stereo requested, plays */
    if ( (def->compression != _sound_compression_xbox_adpcm && def->compression != _sound_compression_ogg
          && def->compression != _sound_compression_none)
      || ((def->encoding || def->sample_rate) && def->encoding != _sound_encoding_stereo) )
        return -1;

    if ( source->scale == 0.0f && def->scale_lower_bound.gain == 0.0f )
        return result;

    /* random skip: drop a fraction of plays */
    {
        unsigned int *seed = get_global_local_random_seed_address();
        float skip = ((def->scale_upper_bound.skip_fraction - def->scale_lower_bound.skip_fraction) * scale
                      + def->scale_lower_bound.skip_fraction) * def->skip_fraction;
        if ( real_seed_random(seed) <= (double)skip )
            return result;
    }

    {
        float max_distance = sound_definition_get_maximum_distance(definition_index);
        if ( !sound_definition_is_playable(definition_index) )
            return result;
        audible_index = source_audible(source, max_distance);
    }
    if ( audible_index == -1 )
        return result;

    /* promotion / rate-limit bookkeeping */
    /* DEVIATION: collapsed verbatim-inlined copy of sound_definition_promote@0x83715524 (rate-limit/promotion-throttle bookkeeping over promotion_count/runtime_maximum_play_time/runtime_promotion_counter/runtime_promotion_time/promotion_sound.index, disasm-confirmed field-for-field incl. the __CFADD__ negative-clamp tautology); donor's definition_index param is passed through as the host's own live index, not constant-folded, since sound_new_impulse already carries the same tag index in scope */
    promotion = sound_definition_promote(definition_index);

    if ( promotion )
    {
        if ( promotion == 1 )
            return sound_new_impulse(def->promotion_sound.index, source, source_identifier,
                                     track_proc, track_data, track_data_size, is_local_player);
        return -1;
    }

    /* allocate the runtime sound instance */
    sound_index = datum_new(sound_data);
    result = sound_index;
    if ( sound_index == -1 )
        return result;

    snd = DATA_ARRAY_ELEMENT(sound_data, sound_datum, sound_index);

    /* listener distance for the delay / falloff */
    {
        float distance;
        unsigned int mode = (uint16_t)source->spatialization_mode;
        if ( source->spatialization_mode )
        {
            if ( mode == _sound_spatialization_mode_absolute )
            {
                float *listener = sound_manager_globals.listeners[audible_index].matrix.___u1.n[3];
                float dx = listener[0] - source->location.position.n[0];
                float dy = listener[1] - source->location.position.n[1];
                float dz = listener[2] - source->location.position.n[2];
                distance = __builtin_sqrtf(dx * dx + (dz * dz + dy * dy));
            }
            else if ( mode >= NUMBER_OF_SOUND_SPATIALIZATION_MODES )
            {
                distance = source->obstruction;   /* preset distance carried in v58 */
            }
            else
            {
                float x = source->location.position.n[0];
                float y = source->location.position.n[1];
                float z = source->location.position.n[2];
                distance = __builtin_sqrtf(z * z + (x * x + y * y));
            }
        }
        else
        {
            distance = 0.0f;
        }
        distance_delay = (int)(distance * 8.9647064f);
    }

    snd->is_local_player = 0;

    /* first-person sounds (HUD/weapon classes, and dialog outside cinematics) bypass spatialization */
    if ( is_local_player )
    {
        /* First-person weapon/footstep classes always bypass spatialization; weapon_ready only
         * outside cinematics (DB enum $3ECE4FFB...551BA... via sound_class.h). */
        switch ( def->class_index )
        {
            case _sound_class_weapon_fire:
            case _sound_class_weapon_reload:
            case _sound_class_weapon_empty:
            case _sound_class_weapon_charge:
            case _sound_class_weapon_overheat:
            case _sound_class_weapon_idle:
            case _sound_class_footstep:
                snd->is_local_player = 1;
                break;
            case _sound_class_weapon_ready:
                if ( !cinematic_globals->cinematic_skip_in_progress && !cinematic_globals->cinematic_in_progress )
                    snd->is_local_player = 1;
                break;
            default:
                break;
        }
    }
    if ( cinematic_globals->cinematic_in_progress )
    {
        if ( source->spatialization_mode )
        {
            int c = def->class_index;
            if ( c == _sound_class_scripted_dialog_to_player || c == _sound_class_scripted_dialog_to_other )
                snd->is_local_player = 1;
        }
    }

    snd->listener_index = audible_index;
    snd->playing_channel_index = -1;
    snd->type = _sound_impulse;
    snd->definition_index = definition_index;

    {
        unsigned int *seed = get_global_local_random_seed_address();
        double rand_pitch = real_seed_random_range(seed, def->pitch_lower_bound, def->pitch_upper_bound);
        float scaled_pitch = (float)rand_pitch *
            ((def->scale_upper_bound.pitch - def->scale_lower_bound.pitch) * source->scale + def->scale_lower_bound.pitch);
        snd->source_identifier = source_identifier;
        snd->flags = 0;
        snd->pitch = scaled_pitch;
    }

    memcpy(&snd->source, source, 0x40u);
    snd->track_proc = track_proc;
    if ( track_proc )
        memcpy(snd->track_data, track_data, track_data_size);

    {
        int16_t pitch_range = sound_definition_find_pitch_range_by_pitch(def, snd->pitch, 0);
        int16_t perm;
        sound_definition *snd_def;
        sound_pitch_range *ranges;
        sound_permutation *permutation_ptr;

        snd->pitch_range_index = pitch_range;
        perm = sound_definition_next_permutation(def, pitch_range, -1);
        snd->permutation_index = perm;
        snd->loop_track_index = -1;
        snd->fade_stop_time = 0;
        snd->fade_start_time = 0;

        /* recovered: manual (char*)&global_tag_instances->base_address + 32*idx ... [39] -> TAG_INSTANCE + pitch_ranges.address */
        snd_def = TAG_GET(sound_definition, snd->definition_index);
        ranges = (sound_pitch_range *)snd_def->pitch_ranges.address;
        permutation_ptr = (sound_permutation *)ranges[snd->pitch_range_index].permutations.address + perm;
        _sound_cache_sound_request(permutation_ptr, 0, 1u, 0);
    }

    if ( distance_delay > 250 )
    {
        snd->start_time = sound_manager_globals.render_time + distance_delay;
        snd->flags |= (1u << _sound_delayed_bit);
        return result;
    }
    snd->start_time = sound_manager_globals.render_time;
    return result;
}
