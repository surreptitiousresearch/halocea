#include <stdint.h>
extern void cheat_all_vehicles(void);

void cheat_f2(uint8_t key_is_down)
{
    if (key_is_down)
        cheat_all_vehicles();
}
