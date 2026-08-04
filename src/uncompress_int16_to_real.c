#include <stdint.h>
/* uncompress_int16_to_real @0x837E8318 — expands a compressed signed 16-bit sample back to a real value:
 * (2*i + 1) / 65536. */

float uncompress_int16_to_real(int16_t i)
{
    return ((float)i * 2.0f + 1.0f) * 0.000015259022f;
}
