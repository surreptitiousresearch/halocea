/* game_state_memory_pool_new @ 0x83684808 — carve a memory_pool out of the
 * persistent game-state arena and checksum the allocation size. */

#include <stdint.h>
#include "headers/game_state_globals.h"

typedef struct memory_pool memory_pool;
extern int memory_pool_allocation_size(int size);
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);
extern void memory_pool_initialize(memory_pool *pool, const char *name, int size);

memory_pool *game_state_memory_pool_new(const char *name, int size)
{
    int alloc_size = memory_pool_allocation_size(size);
    memory_pool *pool = (memory_pool *)((char *)game_state_globals.base_address
                                        + game_state_globals.cpu_allocation_size);
    game_state_globals.cpu_allocation_size += alloc_size;
    crc_checksum_buffer(&game_state_globals.allocation_size_checksum, &alloc_size, 4);
    memory_pool_initialize(pool, name, size);
    return pool;
}
