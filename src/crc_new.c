/* crc_new @ 0x8371BA48
   Initialises a running CRC-32 accumulator to its seed value (all ones). */

#include <stdint.h>

void crc_new(uint32_t *crc_reference)
{
    *crc_reference = 0xFFFFFFFFu;
}
