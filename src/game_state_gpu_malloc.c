/* game_state_gpu_malloc @0x83684728 — hand out a block from the top of the game-state GPU arena.
 * Grows gpu_allocation_size by `size`, returns the arena base minus the running GPU total (plus the
 * fixed 0x448000 leading region), and folds the size into the allocation-size checksum.
 *
 * Deviation: IDA guesses 16 int args from register noise; the disassembly only reads r3/r4/r5, and
 * the body ignores name/type entirely — the true signature is the classic Halo (name, type, size). */

#include <stdint.h>
#include "headers/game_state_globals.h"

extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);

char * game_state_gpu_malloc(const char *name, const char *type, int size)
{
    game_state_globals.gpu_allocation_size += size;
    char *block = (char *)game_state_globals.base_address
        - game_state_globals.gpu_allocation_size + 0x448000;
    crc_checksum_buffer(&game_state_globals.allocation_size_checksum, &size, 4);
    return block;
}
