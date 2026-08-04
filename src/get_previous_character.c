/* get_previous_character @0x83814FC8 — find the character that ends just before *index by decoding forward
 * from the string start (the only safe way to back up over variable-width double-byte characters); set *index
 * to that character's start offset and return its (big-endian-combined) value. */
#include <stdint.h>

extern uint8_t double_byte_character(const unsigned char *string);
uint16_t get_previous_character(const unsigned char *string, int16_t *index)
{
    int16_t position = 0;
    int16_t start;
    int character;
    do
    {
        start = position;
        const unsigned char *current = &string[position];
        if ( double_byte_character(current) )
        {
            position = start + 2;
            character = (unsigned short)((current[0] << 8) | current[1]);
        }
        else
        {
            character = current[0];
            position = start + 1;
        }
    }
    while ( position < *index );
    *index = start;
    return character;
}
