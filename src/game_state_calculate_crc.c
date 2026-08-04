/* game_state_calculate_crc @0x83684A28 — checksum the live actor table (stride 1828) to a single CRC,
 * used to detect game-state desync. */

#include <stdint.h>
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void crc_new(uint32_t *crc_reference);
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);

uint32_t game_state_calculate_crc(void)
{
    uint32_t crc[4];
    data_iterator iterator;
    crc_new(crc);
    data_iterator_new(&iterator, actor_data);
    for ( const void *actor = data_iterator_next(&iterator); actor; actor = data_iterator_next(&iterator) )
        crc_checksum_buffer(crc, actor, 1828);
    return crc[0];
}
