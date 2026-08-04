/* game_variant_is_classic @0x83749658 — tests bit 0x80 of the universal_variant flags word (absolute
 * offset 0x38 within game_variant, i.e. universal_variant.flags), confirmed via disasm
 * (`extrwi r3, r11, 1, 24` extracts the MSB of the flags word's low byte). */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/game_engine_universal_variant_flags.h"

uint8_t game_variant_is_classic(game_variant *variant)
{
    return (variant->universal_variant.flags >> _game_mode_classic_bit) & 1;
}
