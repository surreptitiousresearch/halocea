/* default_render_model_region_permutation_indices @ 0x844A5908 (.data, 80 bytes)
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x00000000
 *   +0x0004 = 0x00000000
 *   +0x0008 = 0x00000000
 *   +0x000C = 0x00000000
 *   +0x0010 = 0x00000000
 *   +0x0014 = 0x00000000
 *   +0x0018 = 0x00000000
 *   +0x001C = 0x00000000
 *   +0x0020 = 0x00000000
 *   +0x0024 = 0x00000000
 *   +0x0028 = 0x00000000
 *   +0x002C = 0x00000000
 *   +0x0030 = 0x00000000
 *   +0x0034 = 0x00000000
 *   +0x0038 = 0x00000000
 *   +0x003C = 0x00000000
 *   +0x0040 = 0x00000000
 *   +0x0044 = 0x00000000
 *   +0x0048 = 0x00000000
 *   +0x004C = 0x00000000
 * default all-zeros region-permutation
 * index array used by render_model when the caller passes no region_permutation_indices (the r20 NULL
 * default at 0x8379930C = default_function_values+0x38). data-bss zero-fill; sized by extent to the next
 * symbol (0x844A5928) = 32 bytes = one index per max model region.
 * Formerly misnamed default_render_model_change_color (real_rgb_color) — that name/type belonged to the
 * r22 (change_colors) default one slot over; the render_model reconstruction had shifted the parameter
 * names by one from arg4 on (deferred-drain B6).
 */
char default_render_model_region_permutation_indices[32];
