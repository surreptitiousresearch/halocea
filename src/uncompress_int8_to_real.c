#include <stdint.h>
/* uncompress_int8_to_real @0x837E82F0 — expands a compressed [0,255] byte back to a [0,1] float (i / 255). */

float uncompress_int8_to_real(uint8_t i)
{
    return (float)i * 0.0039215689f;
}
