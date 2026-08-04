/* align_to_character @ 0x83815050 — snap a byte index to the nearest preceding character
 * boundary, treating double-byte characters as a unit. */

#include <stdint.h>

extern uint8_t double_byte_character(const unsigned char *string);

void align_to_character(const unsigned char *string, int16_t *index)
{
    int16_t pos = 0;

    while ( pos < *index )
    {
        int16_t start = pos;
        if ( double_byte_character(&string[pos]) )
            pos = start + 2;
        else
            pos = start + 1;
    }
    *index = pos;
}
