#include <stdint.h>
/* floor_power2 @0x8379A420 — largest power of two <= x (returns 1 for x < 2). */

signed int floor_power2(uint16_t x)
{
    signed int result = 1;
    if (x >= 2)
    {
        signed int power = 2;
        do
        {
            result = power;
            power *= 2;
        } while (power <= (signed int)x);
    }
    return result;
}
