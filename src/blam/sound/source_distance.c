/* source_distance @0x83715790 — distance associated with a sound source, depending on its spatialization mode:
 * unspatialized (0) -> 0; world-relative (1) -> distance from the given listener's world position; a
 * fixed/preset-distance mode (>=3) -> reads an uninitialized stack slot (confirmed via disasm: `lfs f1,
 * back_chain(r1)`, the caller's stack linkage slot — not a real field); anything else -> magnitude of the
 * source's raw position vector. */

#include <stdint.h>
#include "headers/sound_source.h"
#include "headers/sound_manager_globals.h"
#include "headers/spatialization_mode.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


float source_distance(int16_t listener_index, sound_source *source)
{
    if ( source->spatialization_mode == _sound_spatialization_mode_none )
        return 0.0f;

    if ( source->spatialization_mode == _sound_spatialization_mode_absolute )
    {
        const real_point3d *listener_position = &sound_manager_globals.listeners[listener_index].matrix.___u1.__s1.position;
        float dx = listener_position->n[0] - source->location.position.n[0];
        float dy = listener_position->n[1] - source->location.position.n[1];
        float dz = listener_position->n[2] - source->location.position.n[2];
        return __fsqrts(dx * dx + dy * dy + dz * dz);
    }

    if ( (uint16_t)source->spatialization_mode >= NUMBER_OF_SOUND_SPATIALIZATION_MODES )
    {
        /* Deviation: reproduces a read of uninitialized stack memory (the caller's back-chain slot), verified
         * via disasm — not a real field of anything. */
        float back_chain;
        return back_chain;
    }

    float x = source->location.position.n[0];
    float y = source->location.position.n[1];
    float z = source->location.position.n[2];
    return __fsqrts(x * x + y * y + z * z);
}
