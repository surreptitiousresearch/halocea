#include "headers/input_blob.h"

extern void *memset(void *dst, int value, unsigned int count);

void clear_input_blob(input_blob *input)
{
    /* recovered: 8-dword pun-clear loop -> memset over the whole struct */
    memset(input, 0, sizeof(input_blob));
}
