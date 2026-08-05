/* compress_real_to_int8 @0x837E8010 — quantize a [0,1] real to an unsigned 8-bit sample.
 *
 * DEVIATION: decompiler shows >> 24 (wrong); disasm: fctiwz + lwz at back_chain+4 + clrlwi(24) = & 0xFF */
#include <stdint.h>

uint8_t compress_real_to_int8(float z)
{
    return (uint8_t)(int)(z * 255.0f);
}
