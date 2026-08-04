/* action_obey_individual_advance @0x837DEDC0 — per-individual command-list iterator callback (advance form):
 * forces the individual to re-read the current command by clearing the metadata sub-state (bits 0x18) and
 * setting the "advance" bit (0x08). Only simple_control is used; the other iterator parameters are ignored. */

#include <stdint.h>
#include "headers/obey_individual_simple_control.h"
#include "headers/obey_individual_complex_control.h"
#include "headers/obey_metadata_flags.h"

void action_obey_individual_advance(int actor_index, int unit_index, int16_t command_list_index, obey_individual_simple_control *simple_control, obey_individual_complex_control *complex_control, void *user_data)
{
    /* 0xE7 = ~0x18 clears told_to_advance + waiting_for_advance_notification; |8 re-sets told_to_advance. */
    simple_control->metadata_flags =
        (simple_control->metadata_flags
            & ~((1u << _obey_metadata_told_to_advance_bit) | (1u << _obey_metadata_waiting_for_advance_notification_bit)))
        | (1u << _obey_metadata_told_to_advance_bit);
}
