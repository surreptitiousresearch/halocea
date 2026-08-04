/* cache_files_get_checksum @0x836F6F68 — return the checksum stored in the open cache file's header. */

#include <stdint.h>
#include "headers/cache_file_runtime_globals.h"

/* attested: uint32_t — checksum is a width-pinned 32-bit CRC (lwz of header.checksum) */
uint32_t cache_files_get_checksum(void)
{
    return cache_file_globals.header.checksum;
}
