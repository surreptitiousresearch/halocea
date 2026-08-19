#include <stdint.h>
/* hs_not @0x837F7778 — logical NOT of a boolean (HaloScript `not` builtin). */

uint8_t hs_not(uint8_t value)
{
    return value == 0;
}
