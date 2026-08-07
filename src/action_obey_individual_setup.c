/* action_obey_individual_setup @0x837DEB30 — per-individual command-list iterator callback (setup form): reset an
 * individual's simple control to the start of the command list, then optionally reset its complex control. The
 * "initiative" flag passed via user_data sets simple_control bit 0x01.
 *
 * DEVIATION: the decompiler renders the simple_control reset as a 9-dword unrolled loop over a back-referenced
 * address (&simple_control[-1].directmovement.start_position.n[2]); that address resolves to simple_control-4 and
 * the loop writes 9 dwords = the full 36-byte simple_control, i.e. a plain zero-init, reproduced here as memset. */

#include <stdint.h>
#include <string.h>
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/obey_metadata_flags.h"


void action_obey_individual_setup(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, uint8_t *user_data)
{
    memset(simple_control, 0, sizeof(*simple_control));
    simple_control->current_command_index = -1;
    if ( *user_data )
        simple_control->metadata_flags |= (1u << _obey_metadata_targeting_bit);
    else
        simple_control->metadata_flags &= ~(1u << _obey_metadata_targeting_bit);

    if ( complex_control )
    {
        memset(complex_control, 0, sizeof(*complex_control));
        complex_control->override_movement_type = -1;
    }
}
