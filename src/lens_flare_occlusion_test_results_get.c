/* lens_flare_occlusion_test_results_get @0x83791ED0 */
/* lens_flare_occlusion_test_results_get 0x83791ED0 — return a pointer to the per-window occlusion-result byte(s)
 * for one submitted lens flare. The light_index's top bit selects which result pool: dynamic lights (top bit
 * set) index the large results2 pool keyed by (light_index<<16 | lens_flare_index); static lights index the
 * smaller results pool by light_index, then by lens_flare_index within an 18-byte-per-light row. In both pools
 * the final byte is selected by the compressed window index with its high bit (the first-person-weapon-marker
 * flag; set only in lights_preprocess_scene) masked off.
 *
 * The two result pools are modeled in the codebase as flat 2D byte arrays; the static-light row's 2-byte header
 * and [lens_flare_index][window] sub-array are addressed directly here (the decompiler showed a ".data" member). */

#include <stdint.h>
#include "headers/rasterizer_lens_flare_submit_parameters.h"
#include "headers/lens_flare_window_index.h"
#include "headers/blam_data_globals.h"


uint8_t *lens_flare_occlusion_test_results_get(
    const rasterizer_lens_flare_submit_parameters *lens_flare_parameters)
{
    int light_index = (uint16_t)lens_flare_parameters->light_index;
    unsigned int compressed_window_index = lens_flare_parameters->compressed_window_index;

    if ( (light_index & 0xFFFF8000) != 0 )
        return &local_lens_flare_occlusion_test_results2
                    [((light_index << 16) & 0x7FFF0000) | lens_flare_parameters->lens_flare_index]
                    [LENS_FLARE_WINDOW_INDEX(compressed_window_index)];

    /* Generated global is flat [16128] = 896 lights * 18-byte rows; index it 2D via a row-pointer cast. */
    return &((uint8_t (*)[18])local_lens_flare_occlusion_test_results)[(int16_t)light_index]
                [2 + 2 * lens_flare_parameters->lens_flare_index + LENS_FLARE_WINDOW_INDEX(compressed_window_index)];
}
