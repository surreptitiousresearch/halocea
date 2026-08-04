/* scenario_get_sound_environment @0x83704908 — determine the active sound environment for this frame.
 * Scans both local players' camera clusters, picking the highest-priority sound environment among the
 * cluster's fog (underwater) environment and its own sound environment palette entry. The result is
 * eased into scenario_globals->sound_environment_interpolator (per-parameter clamped step), unless the
 * underwater state flipped, in which case it snaps and reports crossed_water_boundary.
 *
 * Fog region element (40 bytes): fog palette index @36 (word[18]). Fog palette
 * element (136 bytes): fog tag index @44 (dword[11]). Fog tag (fog_definition): sound_environment.index
 * @0x110, background_sound.index @0x100, underwater = flags bit _fog_definition_is_water_bit. Sound environment palette element
 * (80 bytes): sound environment tag @44 (dword[11]). Background sound palette element (116 bytes):
 * looping sound tag @44 (dword[11]). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/structure_fog_region.h"
#include "headers/structure_fog_palette_entry.h"
#include "headers/fog_definition.h"
#include "headers/fog_definition_flags.h"
#include "headers/structure_sound_environment_palette_entry.h"
#include "headers/structure_background_sound_palette_entry.h"
#include "headers/observer_result.h"
#include "headers/scenario_globals.h"
#include "headers/sound_environment.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);

extern int local_player_get_player_index(int16_t local_player_index);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern int16_t scenario_get_fog_region_index(const location *location, const real_point3d *position);

/* Step `current` toward `target` by at most `max_delta`. Consolidates the repeated clamped-lerp
 * pattern the decompiler emits inline once per sound_environment field. */
static float interpolate_toward(float current, float target, float max_delta)
{
    float delta = target - current;
    if ( delta < -max_delta )
        delta = -max_delta;
    else if ( delta > max_delta )
        delta = max_delta;
    return current + delta;
}

void scenario_get_sound_environment(
        int *background_sound_index,
        int *sound_environment_tag,
        uint8_t *crossed_water_boundary)
{
    int selected_environment = -1;
    int selected_background = -1;
    __int16 best_priority = 0x8000;     /* signed -32768; any real priority outranks it */
    int underwater = 0;
    __int16 local_player_index;
    __int16 i;
    cache_file_tag_instance *tag_instances = global_tag_instances;

    for ( i = 0, local_player_index = 0; i < 2; local_player_index = ++i )
    {
        const observer_result *camera;
        int cluster_index;

        if ( local_player_get_player_index(local_player_index) == -1 )
        {
            tag_instances = global_tag_instances;
            continue;
        }
        camera = observer_get_camera(local_player_index);
        cluster_index = camera->location.cluster_index;
        if ( cluster_index == -1 )
        {
            tag_instances = global_tag_instances;
            continue;
        }

        {
            structure_bsp *bsp = global_structure_bsp;
            structure_cluster *cluster = (structure_cluster *)bsp->clusters.address + cluster_index;
            __int16 fog_region_index = scenario_get_fog_region_index(&camera->location, &camera->position);
            int background_index;

            tag_instances = global_tag_instances;

            /* fog (underwater) environment */
            if ( fog_region_index != -1 )
            {
                int fog_palette_index = ((structure_fog_region *)bsp->fog_regions.address)[fog_region_index].fog_palette_index;
                if ( fog_palette_index != -1 )
                {
                    int fog_tag_index = ((structure_fog_palette_entry *)bsp->fog_palette.address)[fog_palette_index].fog.index;
                    if ( fog_tag_index != -1 )
                    {
                        fog_definition *fog = TAG_GET(fog_definition, fog_tag_index);
                        int environment_tag = fog->sound_environment.index;
                        if ( environment_tag != -1
                          && TAG_GET(sound_environment, environment_tag)->priority > best_priority )
                        {
                            selected_environment = fog->sound_environment.index;
                            selected_background = fog->background_sound.index;
                            best_priority = TAG_GET(sound_environment, environment_tag)->priority;
                            underwater = fog->flags & (1u << _fog_definition_is_water_bit);
                        }
                    }
                }
            }

            /* cluster's own sound environment palette entry */
            background_index = cluster->sound_environment_palette_index;
            if ( background_index != -1 )
            {
                int environment_tag = ((structure_sound_environment_palette_entry *)bsp->sound_environment_palette.address)[background_index].sound_environment.index;
                if ( environment_tag != -1
                  && TAG_GET(sound_environment, environment_tag)->priority > best_priority )
                {
                    __int16 cluster_background = cluster->background_sound_palette_index;
                    selected_environment = environment_tag;
                    best_priority = TAG_GET(sound_environment, environment_tag)->priority;
                    underwater = 0;
                    if ( cluster_background == -1 || cluster_background >= bsp->background_sound_palette.count )
                        selected_background = -1;
                    else
                        selected_background = ((structure_background_sound_palette_entry *)bsp->background_sound_palette.address)[cluster_background].background_sound.index;
                }
            }
        }
    }

    {
        sound_environment *target = selected_environment == -1
            ? &default_sound_environment
            : TAG_GET_AT(tag_instances, sound_environment, selected_environment);
        sound_environment *interpolator = &scenario_globals->sound_environment_interpolator;

        if ( (unsigned __int8)underwater != scenario_globals->sound_environment_underwater )
        {
            memcpy(interpolator, target, sizeof(sound_environment));
            scenario_globals->sound_environment_underwater = underwater;
            *crossed_water_boundary = 1;
        }
        else
        {
            interpolator->room_intensity        = interpolate_toward(interpolator->room_intensity,        target->room_intensity,        0.029999999f);
            interpolator->room_intensity_hf     = interpolate_toward(interpolator->room_intensity_hf,     target->room_intensity_hf,     0.029999999f);
            interpolator->room_rolloff_factor   = interpolate_toward(interpolator->room_rolloff_factor,   target->room_rolloff_factor,   0.30000001f);
            interpolator->decay_time            = interpolate_toward(interpolator->decay_time,            target->decay_time,            0.1f);
            interpolator->decay_hf_ratio        = interpolate_toward(interpolator->decay_hf_ratio,        target->decay_hf_ratio,        0.029999999f);
            interpolator->reflections_intensity = interpolate_toward(interpolator->reflections_intensity, target->reflections_intensity, 0.029999999f);
            interpolator->reflections_delay     = interpolate_toward(interpolator->reflections_delay,     target->reflections_delay,     0.090000004f);
            interpolator->reverb_intensity      = interpolate_toward(interpolator->reverb_intensity,      target->reverb_intensity,      0.029999999f);
            interpolator->reverb_delay          = interpolate_toward(interpolator->reverb_delay,          target->reverb_delay,          0.003f);
            interpolator->diffusion             = interpolate_toward(interpolator->diffusion,             target->diffusion,             0.029999999f);
            interpolator->density               = interpolate_toward(interpolator->density,               target->density,               0.029999999f);
            interpolator->hf_reference          = interpolate_toward(interpolator->hf_reference,           target->hf_reference,          600.0f);
            *crossed_water_boundary = 0;
        }
        *background_sound_index = selected_background;
        *sound_environment_tag = selected_environment;
    }
}
