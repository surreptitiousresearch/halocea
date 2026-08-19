/* update_channels @ 0x83719C40 — per-frame update of every active hardware sound channel. For
 * each channel playing a sound it computes the fade/attenuation, updates the channel's 3D
 * position (using the listener-anchored "origin" location for first-person sounds, else the
 * sound's world location, with distance attenuation for spatialized modes), advances the
 * channel's impulse/looping playback, and drives mouth aperture for speech sounds. Channels whose
 * sound has fully faded out are stopped. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_channel_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_class_definition.h"
#include "headers/sound_source.h"
#include "headers/real_point3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_channel_flags.h"
#include "headers/spatialization_mode.h"
#include "headers/blam_data_globals.h"

#include "headers/sound_permutation.h"
#include "headers/sound_attachment_data.h"
extern float sound_calculate_fade(int sound_index);
extern void sound_stop(int sound_index);
extern real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void update_channel_for_looping_sound(int16_t channel_index, float fade);
extern void update_channel_for_impulse_sound(int16_t channel_index, float fade);
extern float sound_definition_get_minimum_distance(int sound_definition_index);
extern float sound_definition_get_maximum_distance(int sound_definition_index);
extern float sound_permutation_get_real_mouth_aperture(const sound_permutation *permutation, int16_t estimated_tick_index);
extern void game_sound_set_mouth_aperture(int object_index, float mouth_aperture);
extern uint8_t track_object_impulse_sound(int object_index, const sound_attachment_data *attachment_data, sound_source *source);


void update_channels(void)
{
    /* listener-anchored location used for first-person sounds (position/forward/velocity) */
    float origin_location[9];
    int i;

    origin_location[0] = global_origin3d->n[0];
    origin_location[1] = global_origin3d->n[1];
    origin_location[2] = global_origin3d->n[2];
    origin_location[3] = global_forward3d->n[0];
    origin_location[4] = global_forward3d->n[1];
    origin_location[5] = global_forward3d->n[2];
    origin_location[6] = global_zero_vector3d->n[0];
    origin_location[7] = global_zero_vector3d->n[1];
    origin_location[8] = global_zero_vector3d->n[2];

    for ( i = 0; i < sound_manager_globals.channel_count; i = (int16_t)(i + 1) )
    {
        sound_channel_datum *channel = &sound_channels[i];
        sound_datum *snd;
        sound_definition *definition;
        double fade;
        int16_t mode;

        if ( channel->sound_index == -1 )
            continue;

        sound_source *source;

        snd = DATA_ARRAY_ELEMENT(sound_data, sound_datum, channel->sound_index);
        source = &snd->source;
        definition = TAG_GET(sound_definition, snd->definition_index);
        fade = sound_calculate_fade(channel->sound_index);

        if ( fade == 0.0 && snd->fade_interpolation_end == 0.0f )
        {
            sound_stop(channel->sound_index);
            channel->sound_index = -1;
            continue;
        }

        if ( channel->type_flags & (1u << _sound_channel_3d_bit) )
        {
            /* hardware-3D channel: hand the position straight to the platform */
            mode = source->spatialization_mode;
            if ( mode == _sound_spatialization_mode_absolute )
            {
                if ( snd->is_local_player )
                    sound_manager_globals.platform->set_channel_location(
                        i, 0, (const sound_location *)origin_location, 0.0f, 0.0f, 0, 0);
                else
                    sound_manager_globals.platform->set_channel_location(
                        i, 1, &source->location,
                        source->obstruction, source->occlusion, 0, 0);
            }
            else if ( mode == _sound_spatialization_mode_relative )
            {
                sound_definition *def = TAG_GET(sound_definition, snd->definition_index);
                sound_manager_globals.platform->set_channel_location(
                    i, 1, &source->location, 0.0f, 0.0f, 0,
                    (uint16_t)def->class_index);
            }
            goto update_playback;
        }

        /* software channel: compute listener-relative position + distance attenuation */
        {
            real_point3d relative;
            relative.n[0] = source->location.position.n[0];
            relative.n[1] = source->location.position.n[1];
            relative.n[2] = source->location.position.n[2];
            mode = source->spatialization_mode;

            if ( mode == _sound_spatialization_mode_absolute )
                matrix4x3_inverse_transform_point(
                    &sound_manager_globals.listeners[snd->listener_index].matrix,
                    &source->location.position, &relative);
            else if ( mode != _sound_spatialization_mode_relative )
                goto update_playback;

            {
                float min_distance = sound_definition_get_minimum_distance(snd->definition_index);
                float max_distance = sound_definition_get_maximum_distance(snd->definition_index);
                float dist = __builtin_sqrtf(relative.n[2] * relative.n[2]
                            + (relative.n[1] * relative.n[1] + relative.n[0] * relative.n[0]));
                float attenuation = 1.0f - (dist - min_distance) / (max_distance - min_distance);
                if ( attenuation >= 0.0f )
                {
                    if ( attenuation > 1.0f )
                        attenuation = 1.0f;
                }
                else
                {
                    attenuation = 0.0f;
                }
                fade = attenuation * fade;
            }
        }

    update_playback:
        if ( snd->type )
            update_channel_for_looping_sound(i, fade);
        else
            update_channel_for_impulse_sound(i, fade);

        if ( sound_classes[definition->class_index].speech )
        {
            /* identity test against a generic callback slot (track_proc is void*-typed): void* pun is faithful */
            if ( (void *)snd->track_proc == (void *)track_object_impulse_sound )
            {
                double aperture = sound_permutation_get_real_mouth_aperture(
                    channel->playing_permutation, (int)channel->estimated_tick_time);
                game_sound_set_mouth_aperture(snd->source_identifier, aperture);
            }
        }
    }
}
