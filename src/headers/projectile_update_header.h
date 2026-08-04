#pragma once
/* _projectile_update_header — the message_delta_processor routing header for projectile_build_update_delta.c /
 * projectile_process_update_delta.c (8 bytes). Decode side reads it as raw custom_header bytes:
 * [4]=baseline_index, [5]=sequence_number, [6]=mode_is_stateless (gates the baseline refresh).
 * Wire-format struct — do not reorder or resize. */

typedef struct _projectile_update_header
{
    int           translated_object_index;
    char          baseline_index;  /* projectile.baseline_index (object +634) */
    char          sequence_number; /* projectile.message_index (object +635), incremented after a successful encode */
    unsigned char mode_is_stateless;
} _projectile_update_header;
