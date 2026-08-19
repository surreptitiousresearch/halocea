/* crc_checksum_buffer @ 0x8371BAB8
   Folds a buffer into a running reflected CRC-32 accumulator. The lookup
   table (polynomial 0xEDB88320) is built on first call. */

#include <stdint.h>
#include "headers/crc_globals.h"

void crc_checksum_buffer(uint32_t *crc_reference, const void *buffer, int buffer_size)
{
    if ( !crc_table_built )
    {
        for ( int index = 0; index < 256; index = (short)(index + 1) )
        {
            unsigned int entry = index;
            for ( int bit = 0; bit < 8; bit = (short)(bit + 1) )
            {
                int low_bit_set = (entry & 1) != 0;
                entry >>= 1;
                if ( low_bit_set )
                    entry ^= 0xEDB88320u;
            }
            crc_table_1[index] = entry;
        }
        crc_table_built = 1;
    }

    unsigned int crc = *crc_reference;
    if ( buffer_size > 0 )
    {
        const unsigned char *current = (const unsigned char *)buffer;
        int remaining = buffer_size;
        do
        {
            crc = (crc >> 8) ^ crc_table_1[(*current++) ^ (unsigned char)crc];
            --remaining;
        }
        while ( remaining );
    }
    *crc_reference = crc;
}
