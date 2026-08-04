#pragma once
/* unit_control_data_entry — one field descriptor in a per-version unit_control_data serialization table
 * (unit_control_data_map[version]), terminated by an entry with size == -1 (12 bytes, types_members-confirmed
 * layout). `bs_def` is unused by recorded_animation_initialize_unit_control's decode path, but IS used by
 * recorded_animation_byteswap_unit_control. */

#include "byte_swap_definition.h"

typedef struct unit_control_data_entry
{
    byte_swap_definition *bs_def;  /* 0x00 */
    int                    size;    /* 0x04 — -1 terminates the table */
    int                    offset;  /* 0x08 — byte offset into unit_control_data, or -1 to skip the copy */
} unit_control_data_entry; /* 12 bytes */
