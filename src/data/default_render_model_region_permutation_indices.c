/* default_render_model_region_permutation_indices @ 0x844A5908 — default all-zeros region-permutation
 * index array used by render_model when the caller passes no region_permutation_indices (the r20 NULL
 * default at 0x8379930C = default_function_values+0x38). data-bss zero-fill; sized by extent to the next
 * symbol (0x844A5928) = 32 bytes = one index per max model region.
 * Formerly misnamed default_render_model_change_color (real_rgb_color) — that name/type belonged to the
 * r22 (change_colors) default one slot over; the render_model reconstruction had shifted the parameter
 * names by one from arg4 on (deferred-drain B6). */

char default_render_model_region_permutation_indices[32];
