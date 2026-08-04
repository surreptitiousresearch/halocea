/* character_in_pattern @0x838150C0 — return whether a character appears in a (possibly double-byte) pattern
 * string. Walks the pattern one logical character at a time — combining two bytes big-endian for double-byte
 * characters — and stops at the first match or the terminating null. */

#include <stdint.h>

extern uint8_t double_byte_character(const unsigned char *string);

uint8_t character_in_pattern(uint16_t character, char *pattern)
{
    int found = 0;
    int done = 0;
    int16_t offset = 0;

    do
    {
        uint8_t *current = (uint8_t *)&pattern[offset];
        uint16_t pattern_character;
        if (double_byte_character(current))
        {
            offset += 2;
            pattern_character = current[1] | (uint16_t)(current[0] << 8);
        }
        else
        {
            pattern_character = current[0];
            offset += 1;
        }

        if (pattern_character)
        {
            if (pattern_character != character)
                continue;
            found = 1;
        }
        done = 1;
    }
    while (!done);

    return found;
}
