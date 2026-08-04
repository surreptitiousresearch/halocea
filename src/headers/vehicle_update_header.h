#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* _vehicle_update_header — the message_delta_processor routing header for a vehicle_build_update_delta.c /
 * vehicle_process_update_delta.c update (12 bytes). Sibling shape to biped_build_update_delta.c's own
 * local header struct, but with two raw per-object byte fields (+1318/+1319) instead of biped's three. */

typedef struct _vehicle_update_header
{
    int           translated_object_index;
    char          baseline_index;  /* vehicle.baseline_index (object +1318); decode reads it as custom_header[4] */
    char          sequence_number; /* vehicle.message_index (object +1319), incremented after a successful encode; custom_header[5] */
    unsigned char mode_is_stateless;
    unsigned int  timestamp;
} _vehicle_update_header;
