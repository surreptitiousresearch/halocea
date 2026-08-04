/* game_state_initialize @ 0x836851B0
   One-time setup of the Blam game-state memory arena. Allocates the arena,
   opens/creates the backing file, then carves the game_state_header off the
   front of the CPU allocation region while accumulating its size into the
   allocation-size CRC. */

#include <stdint.h>
#include "headers/game_state_globals.h"

extern void crc_new(uint32_t *crc_reference);
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);
/* extern corrected to match def (game_state_allocate_buffer.c): address is void*. */
extern void *game_state_allocate_buffer(void *address, unsigned int cpu_size, unsigned int gpu_size);
extern void game_state_create_or_open_file(void);

void game_state_initialize(void)
{
    game_state_header *header;
    int header_size;

    crc_new(&game_state_globals.allocation_size_checksum);

    game_state_globals.base_address =
        game_state_allocate_buffer((void *)0x70000u, 0x408000u, 0x40000u);
    game_state_create_or_open_file();

    header = (game_state_header *)((char *)game_state_globals.base_address +
                                  game_state_globals.cpu_allocation_size);

    header_size = sizeof(game_state_header); // 0x14C
    game_state_globals.cpu_allocation_size += sizeof(game_state_header);
    crc_checksum_buffer(&game_state_globals.allocation_size_checksum,
                        &header_size, 4);

    game_state_globals.header = header;
}
