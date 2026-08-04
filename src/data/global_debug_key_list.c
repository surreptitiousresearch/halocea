/* global_debug_key_list @ 0x841765A8 — .data data-init.
 * DB: debug_key[12] (20 bytes each = 240). Static debug-key binding table; last entry
 * is the {NULL, -1, ...} terminator. Names are the .rdata label strings; callbacks are
 * the cheat_f1..cheat_f11 edge handlers (reloc order preserved). */
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
