/* refresh_listener @ 0x837198C8 — rebuild each local player's sound listener from its camera and push
 * the listener state to the platform. For every local player it marks the listener valid, builds the
 * listener matrix from the camera position/forward/up, brings the camera velocity into listener
 * space, and detects underwater transitions (playing the configured enter/exit splash sound when the
 * state flips). Invalid players' listeners are marked invalid. After both listeners are processed the
 * final listener properties (with the current sound-environment reverb tag) are sent to the platform.
 *
 * global_game_globals->sounds (tag_block @0xF8): address dword 3 = underwater-enter sound, dword 7 =
 * underwater-exit sound. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/platform_sound_listener_properties.h"
#include "headers/observer_globals.h"
#include "headers/sound_source.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

#include "headers/game_globals_tag.h"
#include "headers/tag_reference.h"
#include "headers/spatialization_mode.h"

#include "headers/real_point3d.h"
extern uint8_t game_in_progress(void);
extern int local_player_get_player_index(int16_t local_player_index);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);
extern int sound_new_impulse(int definition_index, sound_source *source, int source_identifier, uint8_t (*track_proc)(int, const void *, sound_source *), const void *track_data, int16_t track_data_size, uint8_t is_local_player);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern real_vector3d *matrix4x3_inverse_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void *memcpy(void *dst, const void *src, unsigned int n);

void refresh_listener(void)
{
    platform_sound_listener_properties properties;
    int local_player;
    int index;

    if ( !game_in_progress() )
        return;

    for ( index = 0, local_player = 0; ; local_player = index )
    {
        sound_listener *listener = &sound_manager_globals.listeners[local_player];

        if ( local_player_get_player_index(local_player) == -1 )
        {
            listener->valid = 0;
        }
        else
        {
            const observer_result *camera = observer_get_camera(local_player);
            unsigned char underwater;

            listener->valid = 1;
            underwater = scenario_location_underwater(&camera->location, &camera->position, 0);
            if ( listener->underwater != underwater )
            {
                sound_source splash;
                int count = global_game_globals->sounds.count;
                int sound_index = -1;

                splash.scale = 1.0f;
                splash.gain = 1.0f;
                splash.spatialization_mode = _sound_spatialization_mode_none;
                if ( underwater )
                {
                    if ( count > 0 )
                        sound_index = ((tag_reference *)global_game_globals->sounds.address)[0].index;
                }
                else if ( count > 1 )
                {
                    sound_index = ((tag_reference *)global_game_globals->sounds.address)[1].index;
                }
                if ( sound_index != -1 )
                    sound_new_impulse(sound_index, &splash, -1, 0, 0, 0, 1u);
            }
            listener->underwater = underwater;

            matrix4x3_from_point_and_vectors(&listener->matrix, &camera->position, &camera->forward,
                                             &camera->up);
            matrix4x3_inverse_transform_vector(&listener->matrix, &camera->velocity, &listener->velocity);

            properties.position.n[0] = listener->matrix.n[3][0];
            properties.position.n[1] = listener->matrix.n[3][1];
            properties.position.n[2] = listener->matrix.n[3][2];
            properties.forward.n[0] = listener->matrix.n[0][0];
            properties.forward.n[1] = listener->matrix.n[0][1];
            properties.forward.n[2] = listener->matrix.n[0][2];
            properties.up.n[0] = listener->matrix.n[2][0];
            properties.up.n[1] = listener->matrix.n[2][1];
            properties.up.n[2] = listener->matrix.n[2][2];
            properties.translational_velocity.n[0] = listener->velocity.n[0];
            properties.translational_velocity.n[1] = listener->velocity.n[1];
            properties.translational_velocity.n[2] = listener->velocity.n[2];
            memcpy(&properties.matrix, &listener->matrix, sizeof(properties.matrix));
            properties.underwater = listener->underwater;
        }

        index = (int16_t)(local_player + 1);
        if ( index >= 2 )
            break;
    }

    properties.sound_environment_tag = sound_manager_globals.sound_environment_tag;
    sound_manager_globals.platform->set_listener_properties(&properties);
}
