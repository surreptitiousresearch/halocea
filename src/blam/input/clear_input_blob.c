/* clear_input_blob @0x836DE648 */
#include <string.h>
#include "headers/input_blob.h"


void clear_input_blob(input_blob *input)
{
    /* recovered: 8-dword pun-clear loop -> memset over the whole struct */
    memset(input, 0, sizeof(input_blob));
}
