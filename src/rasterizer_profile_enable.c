/* rasterizer_profile_enable @0x836A5DC8 — toggle GPU profiling capture. Empty in this release build (the
 * profiler is compiled out); the function body is a bare return. */

#include <stdint.h>

void rasterizer_profile_enable(uint8_t enable)
{
    (void)enable;
}
