/* cheat_f1 @0x836EAE70 */
#include <stdint.h>
extern void cheat_all_weapons(void);

void cheat_f1(uint8_t key_is_down)
{
    if (key_is_down)
        cheat_all_weapons();
}
