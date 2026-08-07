/* file_path_add_name @0x837E6200 — append a name to a path string, inserting a '\' separator if the path is
 * non-empty, and bounding the result to 255 chars + NUL. No-op for an empty name. */
#include <string.h>
void file_path_add_name(char *path, const char *name)
{
    if ( !*name )
        return;

    char *end = path;
    while ( *end++ )
        ;
    int length = end - path - 1;
    char *tail = &path[length];
    if ( tail != path )
    {
        *tail++ = '\\';
        *tail = 0;
    }

    char *end2 = path;
    while ( *end2++ )
        ;
    strncpy(tail, name, 255 - (end2 - path - 1));
    path[255] = 0;
}
