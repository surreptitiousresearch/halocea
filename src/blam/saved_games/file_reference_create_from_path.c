/* file_reference_create_from_path @0x83767D10 — initialize a file_reference: zero it, stamp its header
 * (location magic at [0], drive/path id word = -1 at [6]), then set either a directory path or a file name. */

#include <stdint.h>
#include <string.h>
#include "headers/file_reference.h"

extern void file_path_add_name(char *path, const char *name);
extern file_reference *file_reference_set_name(file_reference *reference, const char *name);

file_reference *file_reference_create_from_path(file_reference *reference, const char *path, uint8_t directory)
{
    memset(reference, 0, sizeof(file_reference));
    reference->info.location = (unsigned short)-1;
    /* DEVIATION: signature was 0x66696C65 ('file'); the binary composes 0x66696C6F ('filo') via
     * lis r11,0x6669 @0x83767D34 + ori r9,r11,0x6C6F @0x83767D3C, stored by stw r9,0(r31) @0x83767D48. */
    reference->info.signature = 0x66696C6Fu;   /* 'filo' location header magic */

    if ( directory )
        file_path_add_name(reference->info.path, path);
    else
        file_reference_set_name(reference, path);

    return reference;
}
