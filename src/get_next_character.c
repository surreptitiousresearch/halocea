/* get_next_character @0x83814F48 — decode the character at *index in a (possibly double-byte) string and
 * advance *index past it. Double-byte characters are returned big-endian-combined (high byte << 8 | low). */
#include <stdint.h>

extern uint8_t double_byte_character(const unsigned char *string);
uint16_t get_next_character(const unsigned char *string, int16_t *index)
{
    const unsigned char *current = &string[*index];
    int16_t position = *index;
    if ( double_byte_character(current) )
    {
        *index = position + 2;
        return (current[0] << 8) | current[1];
    }
    *index = position + 1;
    return current[0];
}
