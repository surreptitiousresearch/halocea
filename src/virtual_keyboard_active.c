/* virtual_keyboard_active @ 0x83783408 — whether the on-screen keyboard is up. */

#include <stdint.h>
#include "headers/virtual_keyboard_globals_t.h"

/* attested uint8_t: callee lbz r3; all 5 callers clrlwi r11,r3,24 (byte-normalization rule) */
uint8_t virtual_keyboard_active(void)
{
    return virtual_keyboard_globals.active;
}
