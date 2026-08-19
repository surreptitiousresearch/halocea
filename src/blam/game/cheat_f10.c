/* cheat_f10 @0x836EAF10 */
#include <stdint.h>
#include "headers/cheats.h"

/* DEVIATION: cntlzw idiom; toggle deathless_player to its boolean inverse.
 * `(cntlzw(x) & 0x20) != 0` is exactly `x == 0` — cntlzw returns 0..32 and 32 (0x20) is the only
 * value in that range with bit 5 set — so the count is reduced to the comparison it implements,
 * the same reduction the rest of the corpus applies to this catalog idiom (action_charge_control.c,
 * blip_type_get.c, actor_look_update.c, …). No __builtin_clz: it is a GCC builtin nothing declares,
 * and the implicit `int()` MSVC invented for it is what the C4013 promotion exists to reject. */
void cheat_f10(uint8_t key_is_down)
{
    if (key_is_down)
        cheat.deathless_player = (unsigned char)(cheat.deathless_player == 0);
}
