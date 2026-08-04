#pragma once
/* _biped_update_header — the message_delta_processor routing header for biped_build_update_delta.c /
 * biped_process_update_delta.c (12 bytes). Sibling shape to _vehicle_update_header
 * (headers/vehicle_update_header.h) plus the pre-clear value of object.force_shield_update.
 * Decode side reads it as raw custom_header bytes: [4]=baseline_index, [5]=sequence_number,
 * [6]=mode_is_stateless (gates the baseline refresh), [7]=force_shield_update. Wire-format struct —
 * do not reorder or resize. */

typedef struct _biped_update_header
{
    int           translated_object_index;
    char          baseline_index;    /* biped.baseline_index (object +1319) */
    char          sequence_number;   /* biped.message_index (object +1320), incremented after a successful encode */
    unsigned char mode_is_stateless;
    char          force_shield_update; /* pre-clear value of object.force_shield_update (+290) */
    unsigned int  timestamp;
} _biped_update_header;
