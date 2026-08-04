/* direction_get_empty @0x8371FEC0 — zero-initialize a direction_specification (type 0, all union bytes 0). */

#include "headers/direction_specification.h"

direction_specification *direction_get_empty(direction_specification *result)
{
    result->type = 0;
    result->_pad02 = 0;
    result->___u1.prop_index = 0;
    result->___u1.vector.n[1] = 0.0f;  /* clears the remaining 8 union bytes (_QWORD store) */
    result->___u1.vector.n[2] = 0.0f;
    return result;
}
