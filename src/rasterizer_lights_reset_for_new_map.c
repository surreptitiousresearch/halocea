#include "headers/blam_data_globals.h"

extern void *memset(void *dst, int value, unsigned int n);
/* rasterizer_lights_reset_for_new_map @0x83792214 — clear the per-frame lens-flare occlusion test results and
 * the active lens-flare count for a new map. */


void rasterizer_lights_reset_for_new_map(void)
{
    memset(local_lens_flare_occlusion_test_results, 0, sizeof(local_lens_flare_occlusion_test_results));
    memset(local_lens_flare_occlusion_test_results2, 0, sizeof(local_lens_flare_occlusion_test_results2));
    local_lens_flare_count = 0;
}
