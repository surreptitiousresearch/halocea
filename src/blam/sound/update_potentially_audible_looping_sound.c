/* update_potentially_audible_looping_sound @0x83713F18 — per-frame service of one active looping sound
 * that is currently in the potentially-audible set. Computes its gain/scale (from a fixed value or the
 * attached object's function), builds its spatialized source, and either refreshes the playing voice or
 * tears the looping sound down when it has gone silent / out of range.
 * Flag bits (game_looping_sound_flags): bit0=unattached ("fixed-scale"), bit1=unattached_stop
 * ("invert" audibility), bit2=unattached_stop_fixed_fadeout ("use-fade"), bit3=alternate.
 * State (game_looping_sound_state): 0 = active (playing), 1 = deactivating (stopping), 2 = inactive (stopped). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/game_sound_globals.h"
#include "headers/game_looping_sound_datum.h"
#include "headers/looping_sound_definition.h"
#include "headers/sound_source.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/location.h"
#include "headers/game_looping_sound_flags.h"
#include "headers/game_looping_sound_state.h"
#include "headers/looping_sound_refresh_state.h"
#include "headers/spatialization_mode.h"
#include "headers/blam_data_globals.h"


extern uint8_t object_get_function_value(int object_index, int16_t function_index, float *value_reference);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern uint8_t sound_refresh_looping(int definition_index, int identifier, sound_source *source, int16_t refresh_state, uint8_t alternate, float force_stop_time);
extern void datum_delete(data_array *data, int index);

void update_potentially_audible_looping_sound(int sound_index, const location *location)
{
    game_looping_sound_datum *sound = DATA_ARRAY_ELEMENT(game_looping_sound_data, game_looping_sound_datum, sound_index);
    int last_frame = sound->last_audible_frame_index;
    looping_sound_definition *definition = TAG_GET(looping_sound_definition, sound->definition_index);
    char serviced_last_frame = (last_frame == -1 || last_frame == game_sound_globals->frame_index - 1);
    int flags = sound->flags;
    int audible;
    real_vector3d angular_velocity;
    sound_source source[2];

    if ( (flags & (1u << _game_looping_sound_unattached_bit)) != 0 )
    {
        source[0].scale = sound->scale;                      /* unattached: fixed scale */
        audible = (~flags >> _game_looping_sound_unattached_stop_bit) & 1;
    }
    else
    {
        audible = object_get_function_value(sound->object_index,
                                            sound->attachment.function_index,
                                            &source[0].scale);
    }

    if ( !audible )
    {
        if ( sound->state == _game_looping_sound_inactive )  /* already stopped */
        {
            sound->last_audible_frame_index = game_sound_globals->frame_index;
            return;
        }
        if ( !serviced_last_frame )
        {
            sound->state = _game_looping_sound_inactive;
            sound->last_audible_frame_index = game_sound_globals->frame_index;
            return;
        }
    }

    {
        int object_index = sound->object_index;
        if ( object_index == -1 )
        {
            source[0].spatialization_mode = _sound_spatialization_mode_none;
        }
        else
        {
            const real_matrix4x3 *node_matrix = object_get_node_matrix(object_index, sound->attachment.node_index);
            matrix4x3_transform_point(node_matrix, &sound->attachment.position, &source[0].location.position);
            matrix4x3_transform_normal(node_matrix, &sound->attachment.forward, &source[0].location.forward);
            object_get_velocities(sound->object_index, &source[0].location.translational_velocity, &angular_velocity);
            source[0].location.game_location = *location;
            source[0].spatialization_mode = _sound_spatialization_mode_absolute;
        }
    }

    source[0].gain = 1.0f;

    if ( audible )
    {
        int16_t mode = (sound->state == _game_looping_sound_active || !serviced_last_frame)
                           ? _looping_sound_refresh_loop : _looping_sound_refresh_start;
        sound->state = _game_looping_sound_active;
        if ( !sound_refresh_looping(sound->definition_index, sound_index, source, mode,
                                    (sound->flags & (1u << _game_looping_sound_alternate_bit)) != 0, 0.0) )
        {
            sound->last_audible_frame_index = game_sound_globals->frame_index;
            return;
        }
        if ( (sound->flags & (1u << _game_looping_sound_unattached_bit)) == 0 )
        {
            sound->state = _game_looping_sound_inactive;
            sound->last_audible_frame_index = game_sound_globals->frame_index;
            return;
        }
        if ( definition->runtime_scripting_sound_index == sound_index )
            definition->runtime_scripting_sound_index = -1;
    }
    else
    {
        if ( serviced_last_frame )
        {
            double fade_time = (sound->flags & (1u << _game_looping_sound_unattached_stop_fixed_fadeout_bit)) != 0 ? 4.0 : 0.0;
            if ( !sound_refresh_looping(sound->definition_index, sound_index, source, _looping_sound_refresh_stop,
                                        (sound->flags & (1u << _game_looping_sound_alternate_bit)) != 0, fade_time) )
            {
                sound->state = _game_looping_sound_deactivating;
                sound->last_audible_frame_index = game_sound_globals->frame_index;
                return;
            }
        }
        if ( (sound->flags & (1u << _game_looping_sound_unattached_bit)) == 0 )
        {
            sound->state = _game_looping_sound_inactive;
            sound->last_audible_frame_index = game_sound_globals->frame_index;
            return;
        }
    }

    datum_delete(game_looping_sound_data, sound_index);
    sound->last_audible_frame_index = game_sound_globals->frame_index;
}
