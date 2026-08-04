/* source_distance_squared @ 0x837156F8 — squared distance from a sound source to a listener.
 * Unspatialized sources return 0; world-spatialized sources measure to the listener's position;
 * otherwise (mode 2) the source position is treated as listener-relative.
 *
 * NOTE (original-binary bug): for spatialization modes >= 3 the original returns an INDETERMINATE
 * value — the frameless leaf loads f1 from the stack back-chain slot (`lfs f1, 0(r1)` @ 0x83715784),
 * i.e. an uninitialized `distance` local. Reconstructed as 0.0f (defined stand-in); see FINDINGS. */

#include <stdint.h>
#include "headers/sound_source.h"
#include "headers/sound_manager_globals.h"

/* attested: r3 extsh on entry => int16_t; r4 read-only pointer => const; f1 single-precision
 * (fmuls/fmadds chains) => float return */
float source_distance_squared(int16_t listener_index, const sound_source *source)
{
    unsigned int mode = (uint16_t)source->spatialization_mode;  /* lhz zero-extend @ 0x837156F8 */
    float result;

    if ( !source->spatialization_mode )
        return 0.0f;

    if ( mode == 1 )
    {
        const float *listener = sound_manager_globals.listeners[listener_index].matrix.n[3];
        float dz = listener[2] - source->location.position.n[2];
        float dx = listener[0] - source->location.position.n[0];
        float dy = listener[1] - source->location.position.n[1];
        result = dy * dy + (dx * dx + dz * dz);
    }
    else if ( mode >= 3 )
    {
        result = 0.0f;  /* deviation: original reads uninitialized stack (see NOTE) */
    }
    else
    {
        float x = source->location.position.n[0];
        float y = source->location.position.n[1];
        float z = source->location.position.n[2];
        result = z * z + (x * x + y * y);
    }
    return result;
}
