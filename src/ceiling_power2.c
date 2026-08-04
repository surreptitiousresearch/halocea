#include <stdint.h>
int ceiling_power2(uint16_t x)
{
    int result = 1;
    if (x > 1u)
    {
        do
            result *= 2;
        while (result < (int)x);
    }
    return result;
}
