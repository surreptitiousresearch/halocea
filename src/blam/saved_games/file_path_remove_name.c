/* file_path_remove_name @0x837E6350 — strip the trailing path component from a path string by scanning back to
 * the last '\' separator (multibyte-aware via get_previous_character/get_next_character) and NUL-terminating
 * there (after the separator). */

#include <stdint.h>

extern uint16_t get_previous_character(const unsigned char *string, int16_t *index);
extern uint16_t get_next_character(const unsigned char *string, int16_t *index);

void file_path_remove_name(char *path)
{
    char *end = path;
    while ( *end++ )
        ;

    int16_t offset = (int16_t)(end - path - 1);
    while ( offset && get_previous_character((const unsigned char *)path, &offset) != 92 )
        ;

    uint16_t next_character = get_next_character((const unsigned char *)path, &offset);
    int16_t cut = offset;
    if ( next_character == 92 )
        cut = offset - 1;
    path[cut] = 0;
}
