/* file_location_get_full_path @0x837E6580 — build a full path from a path component. If the component isn't
 * already an absolute "X:\" path (drive letter, ':', '\'), it is prefixed with the default device root
 * "d:\"; otherwise it is copied verbatim. The `location` argument is unused in this release path. */

#include <stdint.h>

extern char *strcpy(char *dst, const char *src);
extern unsigned short *__pctype_func(void);   /* CRT ctype table accessor */

void file_location_get_full_path(int16_t location, const char *path, char *full_path)
{
    *full_path = 0;
    if ( !path[0] || !path[1] || !path[2]
         || (__pctype_func()[(unsigned char)path[0]] & 0x103) == 0
         || path[1] != ':' || path[2] != '\\' )
    {
        strcpy(full_path, "d:\\");
    }

    const char *src = path;
    char *dst = full_path;
    while ( *dst++ )
        ;
    --dst;
    int c;
    do
    {
        c = (unsigned char)*src++;
        *dst++ = c;
    }
    while ( c );
}
