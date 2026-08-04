#pragma once
/* Lens flare compressed_window_index: bit 7 marks a flare resolved through a first-person weapon
 * marker, the low 7 bits are the render window index. Basis: lights_preprocess_scene sets bit 7
 * when first_person_weapon_get_marker_by_name_render supplies the markers; rasterizer_lens_flares_draw
 * and lens_flare_occlusion_test_results_get mask it off to recover the window index. */

#define LENS_FLARE_FIRST_PERSON_MARKER_FLAG     0x80
#define LENS_FLARE_IS_FIRST_PERSON_MARKER(cwi)  (((cwi) & LENS_FLARE_FIRST_PERSON_MARKER_FLAG) != 0)
#define LENS_FLARE_WINDOW_INDEX(cwi)            ((cwi) & 0x7F)
