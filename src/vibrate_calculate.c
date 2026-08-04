/* vibrate_calculate @0x83739390 — blend a player's controller rumble state into a final left/right motor
 * amplitude pair. Starts from the datum's continuous (looping) frequencies, then adds every still-active
 * impulse's contribution (duration > elapsed) scaled by its transition function evaluated at the
 * remaining-time fraction, clamped to [0,1]. If a scripted vibrate override has a non-zero scale, its own
 * left/right frequencies (scaled) replace the accumulated continuous base instead of adding to it. The
 * final left/right values are scaled to 0..65535 and clamped before being packed into the returned
 * vibrate_data.
 *
 * DEVIATION: the DB prototype confirms a single real parameter — the raw decompile's separate
 * `retstr`/`datum` split is Hex-Rays misdetecting the calling convention for a by-value small-struct return
 * (`vibrate_data` is 4 bytes, packed into one GPR: left_frequency in the high half, right_frequency in the
 * low half). Every read the raw decompile attributed to `retstr` is actually reading `datum` — confirmed by
 * disasm (only r3 is ever touched; the fabricated `datum` parameter is never referenced). Field offsets
 * were cross-checked against the fully-resolved headers/vibrate_global_data.h (used by 9 other already-done
 * vibrate_*.c files), not the older stale headers/vibrate_globals.h (which forward-declares
 * `vibrate_definition` as incomplete and is unused by any other file). */

#include <stdint.h>
#include "headers/vibrate_global_data.h"
#include "headers/vibrate_data.h"

extern float transition_function_evaluate(int16_t function_type, float value);

vibrate_data vibrate_calculate(player_vibrate_datum *datum)
{
    float left = datum->left_frequency_continuous;
    float right = datum->right_frequency_continuous;

    for ( int i = 0; i < 8; ++i )
    {
        float elapsed = datum->time_elapsed[i];

        for ( int channel = 0; channel < 2; ++channel )
        {
            vibrate_frequency_definition *frequency = &datum->impulse[i].vibrate_frequencies[channel];

            if ( frequency->duration > elapsed )
            {
                float t = 1.0f - elapsed / frequency->duration;
                if ( t < 0.0f )
                    t = 0.0f;
                else if ( t > 1.0f )
                    t = 1.0f;

                float contribution = transition_function_evaluate(frequency->fade_function, t) * frequency->frequency;
                if ( channel == 0 )
                    left += contribution;
                else
                    right += contribution;
            }
        }
    }

    if ( vibrate_globals->scripted_vibrate.scale != 0.0f )
    {
        left = vibrate_globals->scripted_vibrate.left_frequency * vibrate_globals->scripted_vibrate.scale + left;
        right = vibrate_globals->scripted_vibrate.right_frequency * vibrate_globals->scripted_vibrate.scale + right;
    }

    float left_scaled = left * 65535.0f;
    if ( left_scaled < 0.0f )
        left_scaled = 0.0f;
    else if ( left_scaled > 65535.0f )
        left_scaled = 65535.0f;

    float right_scaled = right * 65535.0f;
    if ( right_scaled < 0.0f )
        right_scaled = 0.0f;
    else if ( right_scaled > 65535.0f )
        right_scaled = 65535.0f;

    vibrate_data result;
    result.left_frequency = (int)left_scaled;
    result.right_frequency = (int)right_scaled;
    return result;
}
