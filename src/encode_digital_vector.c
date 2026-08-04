/* encode_digital_vector @0x8379D108 — DEVIATION: the DB's own funcs.prototype passes `in` as a real_vector3d
 * BY VALUE, but the decompiler treated the parameter as a pointer and read `v1` (used for the first two
 * bit tests) completely uninitialized, then read `*(float*)&in` (the pointer's own bit pattern reinterpreted
 * as a float) for the last two — both nonsensical. disasm_range(0x8379D108,0x8379D16C) shows the real
 * source: the two 64-bit incoming argument registers are spilled to the stack and reloaded as floats at
 * offsets 0 and +4, i.e. in.n[0] for the first bit-pair and in.n[1] for the second; in.n[2] (spilled but
 * never reloaded) is genuinely unused. Each pair of bits encodes (component > eps, component < -eps). */

#include "headers/real_vector3d.h"

unsigned int encode_digital_vector(real_vector3d in)
{
    unsigned int bits = in.n[0] > 0.0001f;

    bits = (bits << 1) | (in.n[0] < -0.0001f);
    bits = (bits << 1) | (in.n[1] > 0.0001f);
    bits = (bits << 1) | (in.n[1] < -0.0001f);
    return bits;
}
