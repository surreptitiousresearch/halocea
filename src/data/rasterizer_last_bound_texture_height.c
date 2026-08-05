#include <stdint.h>
/* rasterizer_last_bound_texture_height @ 0x844209A2 - .data zero-fill (IDA word_844209A2, no PDB name).
 * Halfword: rasterizer_set_texture_non_blocking writes it with `sth r10,(word_844209A2-0x844209A0)(r30)`
 * (disasm 0x8369CB68); 0x844209A0 is the paired last-bound width. */

int16_t rasterizer_last_bound_texture_height;
