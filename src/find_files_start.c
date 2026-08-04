/* find_files_start @0x837E6180 — begin a recursive directory enumeration: close any handles left open
 * from a previous walk (depth..0), then reset state to depth 0 for the given directory, recording its
 * flags, location id, and path. The path copy is an inline strcpy from the file_reference payload
 * (directory->data+8) into find_files_globals.path (same relative offset). */

#include "headers/find_files_globals.h"
#include "headers/file_reference.h"

extern int CloseHandle(void *handle);

void find_files_start(unsigned int flags, const file_reference *directory)
{
    if ( find_files_globals.depth >= 0 )
    {
        int depth = find_files_globals.depth;
        do
        {
            void *handle = find_files_globals.handles[depth];
            if ( handle != (void *)-1 )
            {
                CloseHandle(handle);
                find_files_globals.handles[depth] = (void *)-1;
            }
            depth = (__int16)(depth - 1);
        }
        while ( depth >= 0 );
    }
    find_files_globals.flags = flags;
    find_files_globals.depth = 0;
    find_files_globals.location = *(__int16 *)&directory->data[6];

    const char *src = &directory->data[8];
    char *dst = find_files_globals.path;
    char c;
    do
    {
        c = *src;
        *dst++ = c;
        ++src;
    }
    while ( c );
}
