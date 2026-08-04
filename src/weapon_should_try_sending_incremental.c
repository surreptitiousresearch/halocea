#include <stdint.h>
/* weapon_should_try_sending_incremental @0x836DAD18 — weapons always attempt to send incremental
 * network updates. */

uint8_t weapon_should_try_sending_incremental(int object_index)
{
    return 1;
}
