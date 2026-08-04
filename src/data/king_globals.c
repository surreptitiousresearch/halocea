/* king_globals @ 0x844B7750 - king of the hill globals (DB 428; ledger 432 gap) (data-bss, 428 bytes).
 * A5 sweep flagged lhz/sth at +0x1AC (== struct size 428, addr 0x844B78FC): that is a SEPARATE UNNAMED
 * 4-byte KotH global in the 4-byte gap before king_engine_hills (0x844B7900), reached via a shared anchor
 * register. king_globals_type is DB types_members-confirmed at 428 (last field hill_timer @ 0x1A8) and no
 * source uses king_globals+0x1AC. Sibling-global access, not a missing trailing field. 2026-07-31. */
#include "../headers/king_globals.h"

king_globals_type king_globals;
