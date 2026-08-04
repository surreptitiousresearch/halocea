/* saved_game_file_generate_checksum @0x83727100 — compute a CRC checksum over a buffer: initialize a fresh
 * CRC accumulator then fold the buffer into it. The accumulator/result is stored at *checksum. */

#include <stdint.h>

extern void crc_new(uint32_t *crc_reference);
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);

void saved_game_file_generate_checksum(const void *buffer, uint16_t buffer_size, uint8_t *checksum)
{
    crc_new((unsigned int *)checksum);
    crc_checksum_buffer((unsigned int *)checksum, buffer, buffer_size);
}
