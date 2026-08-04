/* file_rename @0x837E6938 — rename the file behind a file_reference: build the current full path (location
 * word at data[6], relative path at data[8]), derive the new full path by swapping the trailing name, move
 * the file on disk (Win32 MoveFileA — platform boundary), and on success rewrite the reference's own
 * trailing name. */

#include <stdint.h>
#include "headers/file_reference.h"
#include <string.h>

extern void file_location_get_full_path(int16_t location, const char *path, char *full_path);
extern void file_path_remove_name(char *path);
extern void file_path_add_name(char *path, const char *name);
extern int MoveFileA(const char *existing_name, const char *new_name);   /* Win32 boundary */

uint8_t file_rename(file_reference *file, const char *name)
{
    char current_path[256];
    char new_path[256];
    memset(current_path, 0, 256);
    memset(new_path, 0, sizeof(new_path));

    unsigned char *reference_path = &file->data[8];
    file_location_get_full_path(*(unsigned short *)&file->data[6], reference_path, current_path);

    int i = 0;
    char c;
    do
    {
        c = current_path[i];
        new_path[i++] = c;
    }
    while ( c );

    file_path_remove_name((unsigned char *)new_path);
    file_path_add_name((unsigned char *)new_path, name);

    if ( !MoveFileA(current_path, new_path) )
        return 0;

    file_path_remove_name(reference_path);
    file_path_add_name(reference_path, name);
    return 1;
}
