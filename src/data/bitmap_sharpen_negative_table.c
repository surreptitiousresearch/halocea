#include <stdint.h>
/* bitmap_sharpen_negative_table @ 0x8446A2D0 - .data zero-fill (IDA word_8446A2D0, no PDB name).
 * The negative half of bitmap_sharpen's 256-entry filter pair: bitmap_sharpen indexes it with
 * `sthx r5,r7,r31` where r31 = positive_table + (0x8446A2D0-0x8446A0D0) (disasm 0x8377DD8C/0x8377DDA4) and
 * the loop runs to 0x100, so 256 halfwords - the same shape as src/data/positive_table.c. */

int16_t bitmap_sharpen_negative_table[256];
