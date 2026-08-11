/* game_state_load_core @0x836852E8 — load a named game-state core file: validate its header, then (on a valid
 * header) run the before-load procs, read the arena (4489216 bytes), and run the after-load procs. Reports the
 * outcome to the console.
 *
 * DEVIATION: the disassembly sets only r3 for game_state_header_valid; the 2nd arg is a stale register, not a
 * meaningful flag. Nothing writes r4 between the preceding `bl game_state_read_core_header` @0x83685304 and
 * the `bl game_state_header_valid` @0x83685318 (the only instruction in that window is
 * `addi r3, r1, header` @0x83685314), and the callee never reads it: all 61 instructions of
 * game_state_header_valid, 0x83684CA8-0x83684D98, mention r4 zero times. The argument is provably dead, so it
 * is written as the literal 0 both siblings use — game_state_try_and_load_from_persistent_storage.c:28 and
 * hcex/hcex_checkpoint_read_header.cpp:34, whose call sites have the identical shape. This file previously
 * modelled it as an uninitialized local, which reads the same to a human and is undefined behaviour to a
 * compiler. */

#include <stdint.h>
#include "headers/game_state_globals.h"
#include "headers/game_state_header.h"

extern uint8_t game_state_read_core_header(const char *name, void *header, unsigned int header_size);
extern uint8_t game_state_header_valid(game_state_header *header, uint8_t fatal);
extern void game_state_call_before_load_procs(void);
extern void game_state_read_core(const char *name, void *buffer, unsigned int buffer_size);
extern void game_state_call_after_load_procs(void);
extern void console_printf(uint8_t clear, const char *format, ...);

void game_state_load_core(const char *name)
{
    game_state_header header;

    if ( game_state_read_core_header(name, &header, 332)
      && game_state_header_valid(&header, 0) )   /* 2nd arg stale in caller */
    {
        game_state_call_before_load_procs();
        game_state_read_core(name, game_state_globals.base_address, 4489216);
        console_printf(0, "loaded '%s'", name);
        game_state_call_after_load_procs();
    }
    else
    {
        console_printf(0, "couldn't open '%s'", name);
    }
}
