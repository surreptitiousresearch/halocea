/* string_hash @ 0x836F5AE0
   Computes a CRC-32 checksum over the characters of a NUL-terminated string. */

#include <stdint.h>

extern void crc_new(uint32_t *crc_reference);
extern void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size);

unsigned int string_hash(const char *string)
{
    unsigned int crc[4];
    const char *end;

    crc_new(crc);
    end = string;
    while ( *end++ )
        ;
    crc_checksum_buffer(crc, string, (int)(end - string - 1));
    return crc[0];
}
