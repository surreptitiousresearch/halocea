/* data_encode_string @0x8383084C — encode a null-terminated string (clamped to maximum_string_length) into the
 * buffer, writing the characters followed by a terminator. Sets the overflow flag and fails if it would not
 * fit. Returns no-overflow. */

#include <stdint.h>
#include "headers/data_encoding_state.h"
extern char *strncpy(char *dst, const char *src, unsigned int n);

extern unsigned int strnlen(const char *s, unsigned int maxlen);

int data_encode_string(data_encoding_state *state, const char *source_string, int16_t maximum_string_length)
{
    int16_t length = strnlen(source_string, maximum_string_length);
    int offset = state->offset;
    char *destination = (char *)state->buffer + offset;
    if ( length + offset + 1 > state->buffer_size || state->overflow_flag )
    {
        state->overflow_flag = 1;
        return 0;
    }
    strncpy(destination, source_string, length);
    destination[length] = 0;
    int overflow_flag = state->overflow_flag;
    state->offset += length + 1;
    return overflow_flag == 0;
}
