/* cheat_f5 @0x836EAEC0 */
#include <stdint.h>
extern void cheat_teleport_to_camera(void);

void cheat_f5(uint8_t key_is_down)
{
    if (key_is_down)
        cheat_teleport_to_camera();
}
