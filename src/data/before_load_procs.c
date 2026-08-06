/* before_load_procs @ 0x84172064 (.data, 4 bytes)
 * DB applied_types: void (__fastcall *before_load_procs[1])();
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x83713810 -> game_sound_clear
 * before_load_procs (0x84172064, 4 bytes = 1 entry).
 * Deviation: game_sound_clear's int return is decompiler ABI noise; binary calls it
 * void-no-arg, so it is stored cast to game_state_proc.
 */
#include "../headers/blam_data_globals.h"
#include "../headers/game_state_procs.h"

void (*before_load_procs[1])(void) = { (game_state_proc)game_sound_clear };
