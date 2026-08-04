#include <stdint.h>
#include "headers/data_encoding_state.h"

extern char *data_decode_memory(data_encoding_state *state, int16_t count, int code);

unsigned __int64 data_decode_int64(data_encoding_state *state)
{
    unsigned __int64 *value = (unsigned __int64 *)data_decode_memory(state, 1, -8);

    if (value)
        return *value;

    return 0;
}
