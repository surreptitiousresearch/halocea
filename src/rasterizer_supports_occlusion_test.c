/* rasterizer_supports_occlusion_test @0x83785F30 — getter for the occlusion-test support flag (see
 * rasterizer_occulsion_test_initialize.c / rasterizer_occlusion_globals.h for the same global). */

#include <stdint.h>
#include "headers/rasterizer_occlusion_globals.h"

uint8_t rasterizer_supports_occlusion_test(void)
{
    return supports_occlusion_test;
}
