#pragma once
/* _weapon_update_header — the message_delta_processor routing header for weapon_build_update_delta.c /
 * weapon_process_update_delta.c (8 bytes). Decode side reads it as raw custom_header bytes:
 * [4]=baseline_index, [5]=sequence_number, [6]=mode_is_stateless (gates the baseline refresh).
 * Wire-format struct — do not reorder or resize. */

typedef struct _weapon_update_header
{
    int           translated_object_index;
    char          baseline_index;  /* weapon.baseline_index (object +737) */
    char          sequence_number; /* weapon.message_index (object +738), incremented after a successful encode */
    unsigned char mode_is_stateless;
} _weapon_update_header;
