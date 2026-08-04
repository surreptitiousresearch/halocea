/* file_path_split @0x837E63F0 — split a path in place into its components by scanning backwards. The four
 * out-pointers (directory, parent_directory, filename, extension) are first set to the end of the string, then
 * moved as '.' (extension) and '\\' (path) separators are found and NUL'd. `has_filename` selects whether the
 * trailing component is treated as a filename. Mirrors the decompiler's parameter naming. */

#include <stdint.h>

extern uint16_t get_previous_character(const unsigned char *string, int16_t *index);

void file_path_split(
        char *path,
        char **directory,
        char **parent_directory,
        char **filename,
        char **extension,
        uint8_t has_filename)
{
    char *p = path;
    while ( *p++ )
        ;
    int16_t offset = (int16_t)((uint16_t)(p - path - 1));   /* strlen */
    char *end = &path[offset];
    *directory = end;
    *parent_directory = end;
    *filename = end;
    *extension = end;

    if ( offset != 0 )
    {
        do
        {
            int previous = get_previous_character((unsigned char *)path, &offset);
            if ( previous == '.' )
            {
                if ( has_filename && !**filename && !**extension )
                {
                    path[offset] = 0;
                    *extension = &path[offset + 1];
                }
            }
            else if ( previous == '\\' )
            {
                if ( !has_filename || **filename )
                {
                    if ( !**parent_directory )
                        *parent_directory = &path[offset + 1];
                }
                else
                {
                    path[offset] = 0;
                    *filename = &path[offset + 1];
                }
            }
        }
        while ( offset );
    }

    if ( !has_filename || **filename )
    {
        if ( *filename != path )
            *directory = path;
    }
    else
    {
        *filename = path;
    }
}
