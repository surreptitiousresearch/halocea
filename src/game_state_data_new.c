/* game_state_data_new @ 0x83684790 — carve a data_array out of the game-state arena */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_state_globals.h"

extern int data_allocation_size(int16_t maximum_count, int16_t size);
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);
extern void data_initialize(data_array *data, const char *name, int16_t maximum_count, int16_t size);

data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size)
{
    int alloc_size = data_allocation_size(maximum_count, size);
    data_array *array = (data_array *)((char *)game_state_globals.base_address
                                       + game_state_globals.cpu_allocation_size);
    game_state_globals.cpu_allocation_size += alloc_size;
    crc_checksum_buffer(&game_state_globals.allocation_size_checksum, &alloc_size, 4);
    data_initialize(array, name, maximum_count, size);
    return array;
}
