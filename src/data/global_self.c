/* global_self @ 0x841855A0 — data-init pointer (4 bytes) to the simple decompressor
 * worker state. Binary bytes 0x844A6928 big-endian = &decompress_globals. */
#include "../headers/simple_decompressor_definition.h"

extern simple_decompressor_definition decompress_globals; /* 0x844A6928 */
simple_decompressor_definition *global_self = &decompress_globals;
