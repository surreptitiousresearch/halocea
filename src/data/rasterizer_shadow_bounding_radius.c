/* rasterizer_shadow_bounding_radius @ 0x844A5ADC (.data, 56 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x00000000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0x00000000
 *   +0x0020 = 0x00000000
 *   +0x0024 = 0x00000000
 *   +0x0028 = 0x00000000
 *   +0x002C = 0x00000000
 *   +0x0030 = 0x00000000
 *   +0x0034 = 0x00000000
 * .data zero-fill (IDA dword_844A5ADC, no PDB name).
 * _rasterizer_environment_shadow_begin latches the object bounding radius here by spilling f31 to the stack
 * and copying the word back (disasm 0x837A39F4 `stfs f31,var_A0(r1)` / 0x837A3A04 `stw r10,(dword_844A5ADC-
 * 0x844A5B19)(r30)`); _rasterizer_environment_shadow_draw reads it as 1.0f / radius, so a float.
 */
float rasterizer_shadow_bounding_radius;
