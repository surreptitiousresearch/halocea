/* multiple @ 0x82126538 — .rdata, 20 bytes = 5 x int32_t. The editor debug camera's
 * speed-multiplier table, cycled by src/editor_camera_bump_speed.c ("speed is now x%f").
 * Initializer reconstructed from the binary (big-endian words):
 *   +0x0000 0x00000001 ->  1
 *   +0x0004 0x00000005 ->  5
 *   +0x0008 0x00000014 -> 20
 *   +0x000C 0x00000028 -> 40
 *   +0x0010 0x0000003C -> 60
 * The 5-element extent is exact (next named .rdata symbol `white` @0x8212654C) and matches the
 * consumer's `index = (index + 1) % 5`. No enum covers these values (enum_oracle --lookup
 * multiple 1/5/20/40/60: no candidate above noise) — they are literal speed multipliers.
 *
 * DEVIATION: src/editor_camera_bump_speed.c declares `extern float multiple[5]`. The elements
 * are signed 32-bit INTEGERS, not floats. editor_camera_bump_speed @0x837C3AE0 reads the table
 * with `lwzx` and then `extsw` / `fcfid` / `frsp` — an explicit integer-to-float conversion —
 * before `stfs f1, speed`. A float array would have been read with `lfsx` and no conversion,
 * and these bit patterns interpreted as floats are denormals near 1.4e-45. The definition
 * follows the disassembly; the consumer declaration is left untouched.
 *
 * DEVIATION: the same declaration is also missing const — the object is in .rdata
 * (segment perm 4, read-only). */

#include <stdint.h>

const int32_t multiple[5] = { 1, 5, 20, 40, 60 };
