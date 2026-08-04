/* player_profile_get_rgb_color @0x83764F78 — looks up a player-profile color-index's RGB color, clamping
 * the index to the table's [0, 17] range.
 *
 * DEVIATION: the decompiler flagged "local variable allocation has failed" and produced bogus __int64
 * splitting logic. disasm_range(0x83764F78, 0x83765014) shows the clamp is a manual bit-trick
 * (subf/xoris/addc/subfe/and) equivalent to `index < 0 ? 0 : index`, only reached when index < 17 (else
 * clamped straight to 17); the packed 0x00RRGGBB table entry's bytes are then extracted and scaled by
 * 1/255 into result->n[0..2] = {R, G, B}. Reconstructed positionally from disasm. */

#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"


real_rgb_color *player_profile_get_rgb_color(real_rgb_color *result, int index)
{
    int clamped_index = index;
    if (clamped_index >= 17)
        clamped_index = 17;
    else if (clamped_index < 0)
        clamped_index = 0;

    unsigned int color = profile_color_table[clamped_index];
    const float inv255 = 0.0039215689f;

    result->n[0] = (float)((color >> 16) & 0xFF) * inv255;
    result->n[1] = (float)((color >> 8) & 0xFF) * inv255;
    result->n[2] = (float)(color & 0xFF) * inv255;
    return result;
}
