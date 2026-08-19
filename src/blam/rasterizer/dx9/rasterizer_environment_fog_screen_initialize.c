/* rasterizer_environment_fog_screen_initialize @ 0x8378FA20
   No-op success stub. */

#include <stdint.h>

/* uint8_t return (was int): caller normalizes with clrlwi r3,24 before testing
   (@8369BCB8) — an int return would be tested on the full register. */
uint8_t rasterizer_environment_fog_screen_initialize(void)
{
    return 1;
}
