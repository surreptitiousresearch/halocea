/* file_path_add_extension @0x837E62A8 — append ".<extension>" to a (max 255-char) path in place. A dot is
 * inserted only when the path is non-empty; the copy is length-limited and the buffer is always NUL-terminated
 * at [255].
 *
 * Deviation: the decompiler inlines two strlen byte-loops; reproduced with a strlen helper for clarity. */

#include <string.h>

static int path_length(const char *s)
{
    const char *p = s;
    while ( *p++ )
        ;
    return p - s - 1;
}

void file_path_add_extension(char *path, const char *extension)
{
    if ( !*extension )
        return;

    char *end = &path[path_length(path)];
    if ( end != path )
    {
        *end++ = '.';
        *end = 0;
    }
    strncpy(end, extension, 255 - path_length(path));
    path[255] = 0;
}
