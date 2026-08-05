/* track_loop_impulse_sound @ 0x837161F8 — the per-frame position/tracking callback for a looping
 * sound's detail one-shot. Re-reads the parent looping sound's current obstruction/occlusion and (if
 * it has a valid spatial fix) its forward/velocity/BSP location into the impulse's source, applies the
 * detail's random position offset, and — for world-spatialized sounds — re-bases that offset on the
 * looping sound's current position. Returns 0 if the parent looping sound has been freed. */

#include <stdint.h>
#include "headers/sound_source.h"
#include "headers/looping_sound_datum.h"
#include "headers/loop_impulse_sound_tracking_data.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/spatialization_mode.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);

int track_loop_impulse_sound(int looping_sound_index, const loop_impulse_sound_tracking_data *track_data,
                             sound_source *source)
{
    looping_sound_datum *loop = datum_try_and_get(looping_sound_data, looping_sound_index);
    int spatialization_mode;

    if ( !loop )
        return 0;

    source->obstruction = loop->source.obstruction;
    source->occlusion = loop->source.occlusion;
    if ( loop->source.spatialization_mode )      /* has a valid spatial fix */
    {
        source->location.translational_velocity.n[0] = loop->source.location.translational_velocity.n[0];
        source->location.translational_velocity.n[1] = loop->source.location.translational_velocity.n[1];
        source->location.translational_velocity.n[2] = loop->source.location.translational_velocity.n[2];
        source->location.forward.n[0] = loop->source.location.forward.n[0];
        source->location.forward.n[1] = loop->source.location.forward.n[1];
        source->location.forward.n[2] = loop->source.location.forward.n[2];
        source->location.game_location.leaf_index = loop->source.location.game_location.leaf_index;
        /* DEVIATION: the binary copies cluster_index+bonus as one dword; split into the two
         * 16-bit members (same bytes moved) */
        source->location.game_location.cluster_index = loop->source.location.game_location.cluster_index;
        source->location.game_location.bonus = loop->source.location.game_location.bonus;
    }
    else
    {
        source->location.forward = *global_forward3d;
        source->location.translational_velocity = *global_zero_vector3d;
    }

    spatialization_mode = (uint16_t)source->spatialization_mode;
    source->location.position.n[0] = track_data->position_offset.n[0];
    source->location.position.n[1] = track_data->position_offset.n[1];
    source->location.position.n[2] = track_data->position_offset.n[2];
    if ( spatialization_mode == _sound_spatialization_mode_absolute )  /* world: offset from the looping sound's position */
    {
        source->location.position.n[0] = loop->source.location.position.n[0] + source->location.position.n[0];
        source->location.position.n[1] = loop->source.location.position.n[1] + source->location.position.n[1];
        source->location.position.n[2] = loop->source.location.position.n[2] + source->location.position.n[2];
    }
    return 1;
}
