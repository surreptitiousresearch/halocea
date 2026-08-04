/* rasterizer_shaders_initialize @ 0x836A2EF0
   Thin wrapper: succeeds iff rasterizer_shaders_update() succeeds. */

#include <stdint.h>

extern uint8_t rasterizer_shaders_update(void);

uint8_t rasterizer_shaders_initialize(void)
{
    return rasterizer_shaders_update() != 0;
}
