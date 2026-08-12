/* director_process_variables @ 0x836E49B0 — integrate the four debug "director variables" for a local
 * player from this frame's control bits. The wheel/speed input scales a hyper "debug_input_scale"
 * (multiplied by 1.3^speed_delta each frame, clamped to [0.01, 50]); each variable is then accelerated
 * up or down while its control bit is held (with a frame-rate-correct velocity damping), reset to its
 * initial value on its reset bit, integrated into its value, and clamped to [minimum, maximum].
 *
 * The database prototype is (local_player_index, control_bits, float speed_delta); the decompiler's
 * extra a4/a5 args are an artifact of the PPC long-double pow() ABI. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/director_variable_definition.h"
#include "headers/director_variable.h"
#include "headers/blam_data_globals.h"

extern double pow(double base, double exponent);
extern uint8_t game_in_editor(void);

void director_process_variables(int16_t local_player_index, int control_bits, float speed_delta)
{
    director *dir = &director_globals.local_players[local_player_index];
    double scale;
    int i;

    /* hyper input scale: multiply by 1.3^speed_delta, clamp to [0.01, 50] */
    scale = ((float)pow(1.3, speed_delta) * dir->debug_input_scale);
    dir->debug_input_scale = scale;
    if ( scale < 0.0099999998f )
        scale = 0.0099999998f;
    else if ( scale > 50.0f )
        scale = 50.0f;
    dir->debug_input_scale = scale;

    for ( i = 0; i < NUMBER_OF_DIRECTOR_VARIABLES; i = (int16_t)(i + 1) )
    {
        const director_variable_definition *def = &variables[i];
        float *value = &dir->debug_variables[i].value;
        double hyper_scale;
        double damping, retain;
        char negative_held, positive_held, reset_held;
        double damped_velocity;

        /* note: the original indexes variables[0].has_hyper_scale, not variables[i] */
        hyper_scale = variables[_variable_height].has_hyper_scale ? dir->debug_input_scale : 1.0;

        damping = (float)(director_globals.dtime * 5.0);
        if ( damping < 0.0 )       damping = 0.0;
        else if ( damping > 1.0 )  damping = 1.0;
        retain = (1.0f - (float)damping);

        negative_held = (def->negative_bit != -1 && ((1 << def->negative_bit) & control_bits)) ? 1 : 0;
        positive_held = (def->positive_bit != -1 && ((1 << def->positive_bit) & control_bits)) ? 1 : 0;
        reset_held    = (def->reset_bit    != -1 && ((1 << def->reset_bit)    & control_bits)) ? 1 : 0;

        damped_velocity = ((float)retain * dir->debug_variables[i].velocity);
        dir->debug_variables[i].velocity = damped_velocity;

        if ( negative_held && !positive_held )
        {
            dir->debug_variables[i].velocity =
                -((((def->scale * director_globals.dtime) * (float)hyper_scale) * 25.0f)
                       - (float)damped_velocity);
        }
        else if ( positive_held && !negative_held )
        {
            dir->debug_variables[i].velocity =
                ((((def->scale * director_globals.dtime) * (float)hyper_scale) * 25.0f)
                      + (float)damped_velocity);
        }
        else if ( game_in_editor() )
        {
            dir->debug_variables[i].velocity = 0.0f;
        }

        dir->debug_variables[i].delta = director_globals.dtime * dir->debug_variables[i].velocity;

        if ( reset_held )
            *value = def->initial_value;
        else
            *value = *value + (director_globals.dtime * dir->debug_variables[i].velocity);

        if ( *value < def->minimum )
            *value = def->minimum;
        else if ( *value > def->maximum )
            *value = def->maximum;
    }
}
