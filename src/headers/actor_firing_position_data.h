#pragma once
/* actor_firing_position_data — actor_datum member at 0x3B8 (952), 48 bytes. Full layout from an
 * authoritative actor_datum structure dump (ground truth; supersedes the prior partial reconstruction).
 *
 * CORRECTION: the prior version of this header (built from usage evidence alone, before the
 * authoritative dump was available) named the first three fields index / path_unavailable / arrived.
 * The offsets (0x00/0x02/0x03) were right, but two of the names were guesses from local read-site
 * context rather than ground truth: current_position_found_outside_range (0x02) was inferred as
 * "path_unavailable" from actor_active_select_firing_position.c's `!actor[954]` gate, and
 * moved_away_from_firing_position (0x03) was inferred as "arrived" from actor_change_firing_position.c's
 * own top comment ("resets the 'arrived' flag"). Both are now corrected to match the dump. */

#include <stdint.h>
#include "actor_discarded_firing_position.h"
#include "real_point3d.h"

typedef struct actor_firing_position_data
{
    int16_t          current_position_index;                       /* 0x00 (952) — claimed firing position,
                                                                      * -1/0xFFFF none (actor_change_firing_position.c) */
    uint8_t  current_position_found_outside_range;          /* 0x02 (954) */
    uint8_t  moved_away_from_firing_position;                /* 0x03 (955) */
    uint8_t  pursuit_fired_at_orphan;                        /* 0x04 */
    uint8_t  pursuit_communicated_lost_contact;              /* 0x05 */
    unsigned char    _pad06[2];                                       /* 0x06 */
    int              pursuit_prop_index;                              /* 0x08 */
    int16_t          pursuit_positions_count;                          /* 0x0C */
    int16_t          next_discarded_firing_positions_entry;            /* 0x0E */
    actor_discarded_firing_position discarded_firing_positions[4];      /* 0x10 */
    uint8_t  last_discarded_firing_position_valid;              /* 0x20 */
    uint8_t  last_discarded_firing_position_temporary;          /* 0x21 */
    unsigned char    _pad22[2];                                         /* 0x22 */
    real_point3d     last_discarded_firing_position;                    /* 0x24 */
} actor_firing_position_data; /* 0x30 = 48 bytes */
