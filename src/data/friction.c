/* friction @ 0x8211371C -- .rdata, 4 bytes, const float. Initializer reconstructed
 * from the binary (big-endian):
 *   +0x0000 0x40A00000 -> 5.0f
 * Sole reference: the debug-variable velocity damping in
 * src/director_process_variables.c (lfs f13,friction @0x836E4A88; fmuls f0,f0,f13). */

const float friction = 5.0f;
