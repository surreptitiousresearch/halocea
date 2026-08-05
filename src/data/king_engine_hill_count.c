#include <stdint.h>
/* king_engine_hill_count @ 0x844B78FC - .data zero-fill (IDA word_844B78FC, no PDB name).
 * Halfword: find_next_hill reads it with `lhz r30,(word_844B78FC-0x844B7900)(r31)` and sign-extends it
 * (disasm 0x8382BAF8 / 0x8382BB0C-0x8382BB10), so signed. */

int16_t king_engine_hill_count;
