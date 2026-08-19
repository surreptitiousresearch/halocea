/* cheat_f3 @0x836EAEA0 */
#include <stdint.h>
extern void cheat_all_chars(void);

void cheat_f3(uint8_t key_is_down)
{
    if (key_is_down)
        cheat_all_chars();
}
