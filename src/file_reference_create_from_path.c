/* file_reference_create_from_path @0x83767D10 — initialize a file_reference: zero it, stamp its header
 * (location magic at [0], drive/path id word = -1 at [6]), then set either a directory path or a file name. */

#include <stdint.h>
#include "headers/file_reference.h"

extern void *memset(void *dst, int c, unsigned int n);
extern void file_path_add_name(char *path, const char *name);
extern file_reference *file_reference_set_name(file_reference *reference, const char *name);

file_reference *file_reference_create_from_path(file_reference *reference, const char *path, uint8_t directory)
{
    memset(reference, 0, sizeof(file_reference));
    *(unsigned short *)&reference->data[6] = (unsigned short)-1;
    *(unsigned int *)reference->data = 0x66696C65u;   /* 'file' location header magic */

    if ( directory )
        file_path_add_name(&reference->data[8], path);
    else
        file_reference_set_name(reference, path);

    return reference;
}
