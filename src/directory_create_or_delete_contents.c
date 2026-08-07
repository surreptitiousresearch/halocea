/* directory_create_or_delete_contents @0x83767E10 — ensure a directory exists and is empty. Builds a
 * file_reference for the named directory; if it already exists, enumerates and deletes every file in it,
 * otherwise creates the directory.
 * The 0x66696C6F magic and location word (-1) are the file_reference location header written verbatim. */

#include <stdint.h>
#include <string.h>
#include "headers/file_reference.h"

#include "headers/file_last_modification_date.h"
extern void file_path_add_name(char *path, const char *name);
extern uint8_t file_exists(const file_reference *file);
extern void find_files_start(unsigned int flags, const file_reference *directory);
extern uint8_t find_files_next(file_reference *file, file_last_modification_date *date);
extern uint8_t file_delete(file_reference *file);
extern uint8_t file_create(file_reference *file);

void directory_create_or_delete_contents(const char *directory_name)
{
    file_reference directory;
    memset(&directory, 0, sizeof(directory));
    *(int16_t *)&directory.data[6] = -1;
    *(int *)directory.data = 0x66696C6F;   /* file_reference location magic 'filo' */
    file_path_add_name(&directory.data[8], directory_name);

    if ( file_exists(&directory) )
    {
        file_reference entry;
        find_files_start(0, &directory);
        while ( find_files_next(&entry, 0) )
            file_delete(&entry);
    }
    else
    {
        file_create(&directory);
    }
}
