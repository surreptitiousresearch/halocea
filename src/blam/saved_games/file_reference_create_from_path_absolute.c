/* file_reference_create_from_path_absolute @0x83767D80 — initialize a file reference from an absolute path. The
 * reference is tagged with the 'filo' signature and storage location 2 (absolute). When `directory` is set the
 * path is appended as a directory component; otherwise it is set as the file name. */

#include <stdint.h>
#include <string.h>
#include "headers/file_reference.h"
#include "headers/file_reference_location.h"

extern void file_path_add_name(char *path, const char *name);
extern file_reference *file_reference_set_name(file_reference *reference, const char *name);

file_reference *file_reference_create_from_path_absolute(file_reference *reference, const char *path,
                                                         uint8_t directory)
{
    memset(reference, 0, sizeof(file_reference));
    reference->info.location = _file_reference_absolute;
    /* DEVIATION: signature was 0x66696C65 ('file'); the binary composes 0x66696C6F ('filo') via
     * lis r11,0x6669 @0x83767DA4 + ori r9,r11,0x6C6F @0x83767DAC, stored by stw r9,0(r31) @0x83767DB8. */
    reference->info.signature = 0x66696C6Fu; /* 'filo' */
    if (directory)
        file_path_add_name(reference->info.path, path);
    else
        file_reference_set_name(reference, path);
    return reference;
}
