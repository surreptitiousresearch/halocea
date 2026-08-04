/* data_decode_string @0x83830D04 — returns a pointer to the next NUL-terminated string in the decode stream
 * (in place, no copy), or NULL (setting `state->overflow_flag`) if the stream runs out before a terminator is
 * found. `maximum_string_length` is genuinely unused here — confirmed via disasm (r4 is never read) — decode
 * only needs to find the terminator within the buffer; the length cap is enforced on the encode side. */

#include <stdint.h>
#include "headers/data_encoding_state.h"

char * data_decode_string(data_encoding_state *state, int16_t maximum_string_length)
{
    int offset = state->offset;
    int buffer_size = state->buffer_size;
    char *string_start = (char *)state->buffer + offset;

    if (offset >= buffer_size)
    {
        state->overflow_flag = 1;
        return 0;
    }

    int length = 0;
    while (string_start[length])
    {
        length++;
        if (length + state->offset >= state->buffer_size)
        {
            state->overflow_flag = 1;
            return 0;
        }
    }

    state->offset = length + offset + 1;
    return string_start;
}
