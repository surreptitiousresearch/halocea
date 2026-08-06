/* king_globals @ 0x844B7750 (.data, 428 bytes)
 * DB applied_types: king_globals_type king_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 score                      = 0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x40 score_tick                 = 0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x80 on_the_hill                = 0x00000000000000000000000000000000
 *   +0x90 hill_point_count           = 0x00000000
 *   +0x94 hill_points                = 0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x124 convex_hull                = 0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x184 hill_center                = 0x000000000000000000000000
 *   +0x190 hill_state                 = 0x00000000
 *   +0x194 hill_controlled_count      = 0x00000000
 *   +0x198 hill_previous_controller   = 0x00000000
 *   +0x19C hill_top                   = 0x00000000 -> 0f
 *   +0x1A0 hill_bottom                = 0x00000000 -> 0f
 *   +0x1A4 hill_id                    = 0x00000000
 *   +0x1A8 hill_timer                 = 0x00000000
 * king of the hill globals (DB 428; ledger 432 gap) (data-bss, 428 bytes).
 * A5 sweep flagged lhz/sth at +0x1AC (== struct size 428, addr 0x844B78FC): that is a SEPARATE UNNAMED
 * 4-byte KotH global in the 4-byte gap before king_engine_hills (0x844B7900), reached via a shared anchor
 * register. king_globals_type is DB types_members-confirmed at 428 (last field hill_timer @ 0x1A8) and no
 * source uses king_globals+0x1AC. Sibling-global access, not a missing trailing field. 2026-07-31.
 */
#include "../headers/king_globals.h"

king_globals_type king_globals;
