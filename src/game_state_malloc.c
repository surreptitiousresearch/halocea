/* game_state_malloc @ 0x836846C8
   Bump-allocator over the game-state arena: carves `size` bytes off the front
   of the CPU allocation region and folds the size into the allocation CRC. The
   name/type arguments are used by the (compiled-out here) debug tracker.

   The decompiler invents a4..a16 because `size` is re-spilled to a stack slot
   for the crc call; the real prototype is the 3-arg form below. */

#include <stdint.h>
#include "headers/game_state_globals.h"

extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);

void *game_state_malloc(const char *name, const char *type, int size)
{
    char *allocation = (char *)game_state_globals.base_address +
                       game_state_globals.cpu_allocation_size;
    game_state_globals.cpu_allocation_size += size;

    int recorded_size = size;
    crc_checksum_buffer(&game_state_globals.allocation_size_checksum,
                        &recorded_size, 4);
    return allocation;
}
