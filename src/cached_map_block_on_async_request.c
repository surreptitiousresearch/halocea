#include <stdint.h>

extern uint32_t SleepEx(uint32_t ms, int alertable);

int cached_map_block_on_async_request(volatile uint8_t *completion_flag)
{
    if (!*completion_flag)
    {
        while (SleepEx(0x1388u, 1) == 192 && !*completion_flag)
            ;
    }
    return *completion_flag;
}
