#include <stdint.h>

#pragma once
/* DecryptBuffer / decode_1 — TEA (Tiny Encryption Algorithm) CBC-style buffer decryptor used to validate
 * and decrypt shader/asset buffers (e.g. rasterizer_dx9_decrypt_buffer). The 128-bit key is 4 unsigned
 * 32-bit words; each 8-byte block is decrypted with the standard 32-round TEA Feistel schedule (delta
 * 0x9E3779B9) and chained by XOR-ing in the previous ciphertext block (CBC decrypt), matching the
 * disassembly at 0x83688960 (DecryptBuffer) and 0x836888D0 (decode_1). */

/* decode_1 — single 8-byte TEA block decrypt in place. v[0]/v[1] are the two 32-bit halves of the block;
 * k[0..3] is the 128-bit key. */
extern void decode_1(unsigned int *v, const unsigned int *k);

/* DecryptBuffer — decrypts bufferSizeInBytes bytes of buffer in place, 8 bytes at a time, using the
 * 128-bit key (4 unsigned 32-bit words) pointed to by key. */
extern void DecryptBuffer(unsigned int bufferSizeInBytes, uint8_t *buffer, const unsigned int *key);
