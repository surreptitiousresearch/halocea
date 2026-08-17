/* data_decode_int64 @0x83830B00 */
#include <stdint.h>
#include "headers/data_encoding_state.h"

extern char *data_decode_memory(data_encoding_state *state, int16_t count, int code);

uint64_t data_decode_int64(data_encoding_state *state)
{
    uint64_t *value = (uint64_t *)data_decode_memory(state, 1, -8);

    if (value)
        return *value;

    return 0;
}
