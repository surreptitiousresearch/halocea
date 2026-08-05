/* apply_vector_short_difference @0x837F7228 — apply one compressed-vector animation event from a
 * playback stream: the event type (bits 2-7 of the header byte, biased by 15) selects which of up to 3
 * direction vectors (facing/aiming/looking) get updated this tick, via a bitmask (bit0=facing,
 * bit1=aiming, bit2=looking). Facing is always driven straight from the stream if selected. Aiming and
 * looking either copy from an already-updated earlier vector this same tick (facing->aiming,
 * facing->aiming->looking) when that vector was also selected, or are driven from the stream themselves
 * otherwise — but only the *last* vector actually driven from the stream advances the stream pointer
 * (copied vectors don't consume any stream data). Advances *playback_stream past the consumed event.
 *
 * DEVIATION: the decompiler renders `uncompress_vector_from_controller`'s controller argument as an
 * uninitialized local in 3 places; disasm/data-flow shows each is simply the same
 * `&animation_state->{facing,aiming,looking}_control` address just passed to the immediately-preceding
 * update_controller_short call (a plain decompiler failure to re-thread an address it already computed).
 * The DB types *playback_stream as `const char *` (byte-granular), but disasm shows the stored advanced
 * pointer is always `event + sizeof(vector_short_difference_data)` (4 bytes), not +1 — the underlying
 * type is genuinely `const vector_short_difference_data *`, just under-typed in the DB signature. */

#include <stdint.h>
#include "headers/animation_playback_controller.h"
#include "headers/animation_event_header.h"
#include "headers/unit_control_data.h"
#include "headers/vector_short_difference_data.h"
#include "headers/playback_event_type.h"
#include "headers/control_vector_flags.h"

#include "headers/direction_playback_controller.h"
extern void update_controller_short(const vector_short_difference_data *event_data, direction_playback_controller *control);
extern void uncompress_vector_from_controller(real_vector3d *vector, const direction_playback_controller *controller);

void apply_vector_short_difference(animation_playback_controller *animation_state, unit_control_data *control,
    const animation_event_header *header, const char **playback_stream)
{
    const vector_short_difference_data *event = (const vector_short_difference_data *)*playback_stream;

    /* recovered: *(const unsigned char *)header >> 2 -> header->event_type */
    uint8_t event_type = header->event_type - _playback_vector_short_difference_set;
    uint8_t update_facing = event_type & (1u << _control_vector_facing_bit);

    if ( update_facing )
    {
        update_controller_short(event, &animation_state->facing_control);
        uncompress_vector_from_controller(&control->facing_vector, &animation_state->facing_control);
    }

    if ( event_type & (1u << _control_vector_aiming_bit) )
    {
        if ( update_facing )
        {
            animation_state->aiming_control = animation_state->facing_control;
            control->aiming_vector = control->facing_vector;
        }
        else
        {
            update_controller_short(event, &animation_state->aiming_control);
            uncompress_vector_from_controller(&control->aiming_vector, &animation_state->aiming_control);
        }
    }

    if ( event_type & (1u << _control_vector_looking_bit) )
    {
        if ( update_facing )
        {
            animation_state->looking_control = animation_state->facing_control;
            control->looking_vector = control->facing_vector;
            *playback_stream += sizeof(vector_short_difference_data);
            return;
        }

        if ( event_type & (1u << _control_vector_aiming_bit) )
        {
            animation_state->looking_control = animation_state->aiming_control;
            control->looking_vector = control->aiming_vector;
            *playback_stream += sizeof(vector_short_difference_data);
            return;
        }

        update_controller_short(event, &animation_state->looking_control);
        uncompress_vector_from_controller(&control->looking_vector, &animation_state->looking_control);
    }

    *playback_stream += sizeof(vector_short_difference_data);
}
