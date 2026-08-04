/* game_state_load_core @0x836852E8 — load a named game-state core file: validate its header, then (on a valid
 * header) run the before-load procs, read the arena (4489216 bytes), and run the after-load procs. Reports the
 * outcome to the console. */

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
    unsigned char header_valid_flag;   /* decompiler r4: passed-through validity argument */

    if ( game_state_read_core_header(name, &header, 332) && game_state_header_valid(&header, header_valid_flag) )
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
