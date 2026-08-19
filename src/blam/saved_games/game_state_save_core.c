/* game_state_save_core @0x83684BE0 — write the entire game-state arena (4489216 bytes) to a named core file,
 * reporting success or failure to the console. */

#include <stdint.h>
#include "headers/game_state_globals.h"

extern uint8_t game_state_write_core(const char *name, const void *buffer, unsigned int buffer_size);
extern void console_printf(uint8_t clear, const char *format, ...);

void game_state_save_core(const char *name)
{
    if ( game_state_write_core(name, game_state_globals.base_address, 4489216) )
        console_printf(0, "saved '%s'", name);
    else
        console_printf(0, "error writing '%s'", name);
}
