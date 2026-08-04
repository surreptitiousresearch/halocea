/* cache_file_header_verify @0x836F7168 — validate a cache file header: signatures, size bound, name length,
 * and version. Returns 1 if valid, 0 otherwise. The `fatal`/`name` params are unused in this release path. */
#include <stdint.h>
#include "headers/cache_file_header.h"
/* attested: uint8_t return — callers byte-normalize (clrlwi r11, r3, 24 @ 836F7348 et al.);
 * name/fatal are dead params in this release path (r4/r5 never read). */
uint8_t cache_file_header_verify(const cache_file_header *header, const char *name, uint8_t fatal)
{
    if ( header->header_signature != 0x68656164u )   /* 'head' */
        return 0;
    if ( header->footer_signature != 0x666F6F74u )   /* 'foot' */
        return 0;
    if ( (uint32_t)header->size > 0x18000000u )      /* cmplw: unsigned bound check */
        return 0;

    const char *cursor = header->name;
    while ( *cursor++ )
        ;
    if ( (uint32_t)(cursor - header->name - 1) > 0x1F )  /* 32-bit unsigned length check (cmplwi 0x1F) */
        return 0;

    if ( header->version != 7 )
        return 0;
    return 1;
}
