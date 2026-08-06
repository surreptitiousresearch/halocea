/* before_save_procs @ 0x84172060 (.data, 4 bytes)
 * DB applied_types: void (__fastcall *before_save_procs[1])();
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x83684478 -> dummy
 * before_save_procs (0x84172060, 4 bytes = 1 entry).
 */
#include "../headers/blam_data_globals.h"
#include "../headers/game_state_procs.h"

void (*before_save_procs[1])(void) = { dummy };
