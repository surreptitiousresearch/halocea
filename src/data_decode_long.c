/* data_decode_long @0x83830AB8 */
#include <stdint.h>
#include "headers/data_encoding_state.h"

extern char *data_decode_memory(data_encoding_state *state, int16_t count, int code);

int data_decode_long(data_encoding_state *state)
{
    int *value = (int *)data_decode_memory(state, 1, -4);

    if (value)
        return *value;

    return 0;
}
