/* global_debug_key_list @ 0x841765A8 (.data, 240 bytes)
 * DB applied_types: debug_key global_debug_key_list[12];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   [ 0] +0x00 name                       = 0x821138E0 -> "Select This Actor"
 *        +0x04 key_code                   = 0x0001
 *        +0x06 modifier                   = 0x0000
 *        +0x08 function                   = 0x836EAE70 -> cheat_f1
 *        +0x0C toggle_variable            = 0x00
 *        +0x10 variable                   = 0x00000000
 *   [ 1] +0x00 name                       = 0x821138C8 -> "Select Prev Encounter"
 *        +0x04 key_code                   = 0x0002
 *        +0x06 modifier                   = 0x0000
 *        +0x08 function                   = 0x836EAE88 -> cheat_f2
 *        +0x0C toggle_variable            = 0x00
 *        +0x10 variable                   = 0x00000000
 *   [ 2] +0x00 name                       = 0x821138B0 -> "Select Next Encounter"
 *        +0x04 key_code                   = 0x0003
 *        +0x06 modifier                   = 0x0000
 *        +0x08 function                   = 0x836EAEA0 -> cheat_f3
 *        +0x0C toggle_variable            = 0x00
 *        +0x10 variable                   = 0x00000000
 *   [ 3] +0x00 name                       = 0x8211389C -> "Select Next Actor"
 *        +0x04 key_code                   = 0x0004
 *        +0x06 modifier                   = 0x0000
 *        +0x08 function                   = 0x836EAEB8 -> cheat_f4
 *        +0x0C toggle_variable            = 0x00
 *        +0x10 variable                   = 0x00000000
 *   [ 4] +0x00 name                       = 0x82113888 -> "Show Actor Spray"
 *        +0x04 key_code                   = 0x0005
 *        +0x06 modifier                   = 0x0000
 *        +0x08 function                   = 0x836EAEC0 -> cheat_f5
 *        +0x0C toggle_variable            = 0x00
 *        +0x10 variable                   = 0x00000000
 *   [ 5] +0x00 name                       = 0x82113874 -> "Erase All Actors"
 *        +0x04 key_code                   = 0x0006
 *        +0x06 modifier                   = 0x0000
 *        +0x08 function                   = 0x836EAED8 -> cheat_f6
 *        +0x0C toggle_variable            = 0x00
 *        +0x10 variable                   = 0x00000000
 *   ... 6 further elements elided; full hex in .sweep/data_image.tsv
 * .data data-init.
 * DB: debug_key[12] (20 bytes each = 240). Static debug-key binding table; last entry
 * is the {NULL, -1, ...} terminator. Names are the .rdata label strings; callbacks are
 * the cheat_f1..cheat_f11 edge handlers (reloc order preserved).
 */
#include <stdint.h>
#include "headers/debug_key.h"

extern void cheat_f1(uint8_t key_is_down);
extern void cheat_f2(uint8_t key_is_down);
extern void cheat_f3(uint8_t key_is_down);
extern void cheat_f4(uint8_t key_is_down);
extern void cheat_f5(uint8_t key_is_down);
extern void cheat_f6(uint8_t key_is_down);
extern void cheat_f7(uint8_t key_is_down);
extern void cheat_f8(uint8_t key_is_down);
extern void cheat_f9(uint8_t key_is_down);
extern void cheat_f10(uint8_t key_is_down);
extern void cheat_f11(uint8_t key_is_down);

debug_key global_debug_key_list[12] = {
    { .name = "Select This Actor",     .key_code = 1,  .function = cheat_f1  },
    { .name = "Select Prev Encounter", .key_code = 2,  .function = cheat_f2  },
    { .name = "Select Next Encounter", .key_code = 3,  .function = cheat_f3  },
    { .name = "Select Next Actor",     .key_code = 4,  .function = cheat_f4  },
    { .name = "Show Actor Spray",      .key_code = 5,  .function = cheat_f5  },
    { .name = "Erase All Actors",      .key_code = 6,  .function = cheat_f6  },
    { .name = "Profile Reset",         .key_code = 7,  .function = cheat_f7  },
    { .name = "Profile Reset",         .key_code = 8,  .function = cheat_f8  },
    { .name = "Profile Reset",         .key_code = 9,  .function = cheat_f9  },
    { .name = "Fly mode",              .key_code = 10, .function = cheat_f10 },
    { .name = "Saber/Halo mode",       .key_code = 11, .function = cheat_f11 },
    { .name = 0, .key_code = -1 },
};
