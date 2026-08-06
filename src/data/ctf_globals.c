/* ctf_globals @ 0x844A68F0 (.data, 52 bytes)
 * DB applied_types: ctf_globals_type ctf_globals;
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
 * data-bss (52 bytes). ledger size 56 is a next-symbol-gap overcount; DB size 52.
 */
#include "../headers/ctf_globals.h"

ctf_globals_type ctf_globals;
