/* object_impulse_sound_new @0x837148E8 — start a one-shot impulse sound attached to an object node. Builds
 * the attachment data (node + local position/forward) and a default sound source (unit gain, world-space
 * spatialization, invalid cluster), resolves the source against the object via track_object_impulse_sound,
 * and on success spawns the impulse sound, re-tracking it to the object each update. Returns the sound index
 * or -1 if the object could not be resolved.
 *
 * NOTE: the database prototype names the trailing bool is_local_player; the decompiler splits it into an
 * unused 7th parameter and the real 8th. Reconstructed with the single is_local_player parameter, which is
 * what reaches sound_new_impulse. */

#include <stdint.h>
#include "headers/sound_attachment_data.h"
#include "headers/sound_source.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/spatialization_mode.h"

extern uint8_t track_object_impulse_sound(int object_index, const sound_attachment_data *attachment_data, sound_source *source);
extern int sound_new_impulse(int definition_index, sound_source *source, int source_identifier, uint8_t (*track_proc)(int, const void *, sound_source *), const void *track_data, int16_t track_data_size, uint8_t is_local_player);

int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index,
        const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player)
{
    sound_attachment_data attachment;
    sound_source source;

    attachment.node_index = node_index;
    attachment.position.n[0] = position->n[0];
    attachment.position.n[1] = position->n[1];
    attachment.position.n[2] = position->n[2];
    attachment.forward.n[0] = forward->n[0];
    attachment.forward.n[1] = forward->n[1];
    attachment.forward.n[2] = forward->n[2];

    source.gain = 1.0f;
    source.spatialization_mode = _sound_spatialization_mode_absolute;
    source.location.game_location.cluster_index = -1;

    if ( !track_object_impulse_sound(object_index, &attachment, &source) )
        return -1;

    source.scale = scale;
    /* funcptr cast: sound_new_impulse takes a generic const void* track_proc; track_object_impulse_sound
     * is the concrete implementation over sound_attachment_data. Faithful to the original generic-callback
     * registration (not a masked mismatch); the callee stays generic for other track procs. */
    return sound_new_impulse(definition_index, &source, object_index,
            (uint8_t (*)(int, const void *, sound_source *))track_object_impulse_sound,
            &attachment, 28, is_local_player);
}
