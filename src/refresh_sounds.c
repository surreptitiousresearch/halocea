/* refresh_sounds @ 0x83717348 — per-frame maintenance of every active sound. For each sound it stops
 * those whose channel finished a non-looping segment or whose per-frame refresh fails; updates spatial
 * audibility (fading out over 2s when the source becomes inaudible and back in over 0.5s when it
 * returns); flags whether any UI/system-bus dialog (classes 44/46/47) is playing; and, when all
 * players are dead, fades out or stops dialog/announcer sounds. After the pass it eases the global
 * non-dialog gain toward its target (ducked under dialog, otherwise full) at a per-tick rate. */

#include <stdint.h>
#include "headers/sound_manager_globals.h"
#include "headers/sound_datum.h"
#include "headers/sound_definition.h"
#include "headers/sound_source.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/sound_class.h"
#include "headers/sound_flags.h"
#include "headers/sound_type.h"
#include "headers/sound_fade_mode.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern uint8_t players_are_all_dead(void);
extern int data_next_index(const data_array *data, int16_t index);
extern int16_t channel_get_state(int16_t channel_index);
extern uint8_t refresh_sound(uint16_t sound_index);
extern float sound_definition_get_maximum_distance(int sound_definition_index);
extern int16_t source_audible(sound_source *source, float maximum_distance);
extern char *tag_get_name(int16_t tag_index);
extern int sprintf_0(char *string, const char *format, ...);
extern float sound_calculate_fade(uint16_t sound_index);
extern void sound_start_fade(int16_t mode, float seconds, int fade_out_sound_index, int fade_up_sound_index);
extern void sound_stop(int sound_index);

void refresh_sounds(void)
{
    unsigned char all_dead = players_are_all_dead();
    char dialog_playing = 0;
    int i = data_next_index(sound_data, -1);
    float target;
    float rate;
    float delta;

    while ( i != -1 )
    {
        sound_datum *datum = DATA_ARRAY_ELEMENT(sound_data, sound_datum, i);
        int channel = (unsigned __int16)datum->playing_channel_index;
        sound_definition *def = TAG_GET(sound_definition, datum->definition_index);
        int stop = 0;

        /* finished a non-looping channel segment, or per-frame refresh failed while running */
        if ( channel != 0xFFFF && !channel_get_state(channel) )
        {
            int loop_state = datum->type;
            if ( loop_state == _sound_loop_track || loop_state == _sound_stopping_track )
                stop = 1;
        }
        if ( !stop && !refresh_sound(i) && !sound_manager_globals.paused )
            stop = 1;

        if ( !stop )
        {
            sound_source *source = &datum->source; /* non-const: source_audible mutates via compute_sound_obstruction */
            float maximum_distance = sound_definition_get_maximum_distance(datum->definition_index);
            __int16 audibility = source_audible(source, maximum_distance);
            int class_index;
            int faded_out;

            if ( debug_sound )
            {
                char line[544];
                /* recovered: *(float*)(datum+76/80) debug "dx"/"dy" -> source.obstruction/occlusion */
                float obstruction = source->obstruction;
                float occlusion = source->occlusion;
                const char *name = tag_get_name(datum->definition_index);
                sprintf_0(line, "%s|n%f %f", name, obstruction, occlusion);
            }

            class_index = def->class_index;
            if ( class_index == _sound_class_scripted_dialog_to_player
              || class_index == _sound_class_scripted_dialog_to_other
              || class_index == _sound_class_scripted_dialog_force_unspatialized )
                dialog_playing = 1;

            faded_out = datum->flags & (1u << _sound_inaudible_bit);
            if ( audibility == -1 )
            {
                if ( !faded_out )
                {
                    /* inaudible: fade out over 2 seconds */
                    int fade_start = sound_manager_globals.render_time - 1;
                    int fade_end = (int)((float)fade_start + 2000.0f);
                    if ( fade_end <= sound_manager_globals.render_time )
                        fade_end = sound_manager_globals.render_time;
                    datum->fade_interpolation_start = sound_calculate_fade(i);
                    datum->fade_start_time = fade_start;
                    datum->fade_interpolation_end = 0.0f;
                    datum->fade_mode = _sound_fade_mode_linear;
                    datum->fade_stop_time = fade_end;
                    datum->flags |= (1u << _sound_inaudible_bit);
                }
            }
            else
            {
                datum->listener_index = audibility;
                if ( faded_out )
                {
                    sound_start_fade(_sound_fade_mode_linear, 0.5, i, -1);  /* audible again: fade back in */
                    datum->flags &= ~(1u << _sound_inaudible_bit);
                }
            }

            if ( all_dead )
            {
                int dead_class = def->class_index;
                if ( dead_class == _sound_class_scripted_dialog_to_player )
                {
                    if ( channel == 0xFFFF )
                        stop = 1;
                    else
                        sound_start_fade(_sound_fade_mode_linear, 0.30000001, -1, -1);
                }
                else if ( dead_class == _sound_class_scripted_dialog_to_other && channel == 0xFFFF )
                {
                    stop = 1;
                }
            }
        }

        if ( stop )
            sound_stop(i);

        i = data_next_index(sound_data, i);
    }

    /* ease the non-dialog bus toward its target (ducked while dialog plays) */
    if ( dialog_playing )
    {
        rate = sound_manager_globals.ticks_elapsed * 0.029999999f;
        target = sound_gain_under_dialog;
    }
    else
    {
        rate = sound_manager_globals.ticks_elapsed * 0.0070000002f;
        target = 1.0f;
    }
    delta = target - sound_manager_globals.nondialog_gain;
    if ( delta >= -rate )
    {
        if ( delta <= rate )
            rate = delta;
    }
    else
    {
        rate = -rate;
    }
    sound_manager_globals.nondialog_gain = rate + sound_manager_globals.nondialog_gain;
}
