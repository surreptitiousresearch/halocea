/* ctf_globals_baseline @ 0x8465B820 (.data, 52 bytes)
 * DB applied_types: ctf_globals_type ctf_globals_baseline;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 flags                      = 0x0000000000000000
 *   +0x08 weapon_index               = 0x0000000000000000
 *   +0x10 score                      = 0x0000000000000000
 *   +0x18 score_to_win               = 0x00000000
 *   +0x1C flag_warning               = 0x0000
 *   +0x20 flag_warning_time_in_ticks = 0x0000000000000000
 *   +0x28 flag_swap_timer            = 0x00000000
 *   +0x2C non_deterministic_next_flag_failure_time = 0x00000000
 *   +0x30 team_with_flag             = 0x00
 * Storage definition for Blam data-segment global `ctf_globals_baseline` (data-bss, zero-fill).
 */
#include "ctf_globals.h"

/* DB type sizeof=52; ledger size 64 overcounts (next-symbol gap). */
ctf_globals_type ctf_globals_baseline;
