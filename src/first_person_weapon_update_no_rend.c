/* first_person_weapon_update_no_rend @0x8369FCB8 — when normal rendering is disabled
 * (debug_no_drawing) but at least one local player exists, manually drive the first-person weapon
 * update/draw pipeline per camera slot that would otherwise run as part of rendering: prepare the render
 * context for the slot, refresh weapon visibility/pose, and draw with rendering suppressed
 * (hcex_model_rend_disable) so only the hcex bridge's stashed model indices/matrices get updated. In
 * split-screen co-op with a specific local player selected (hcex_coop_local_player_index == 1), slot 1's
 * bridge state is shifted down into slot 0 first (mirrors the later camera-1 update landing in slot 1). A
 * second camera slot is additionally updated when there's more than one local player and no specific
 * co-op local player is selected.
 *
 * DEVIATION: the decompiler shows `first_person_weapon_render_update()`'s return value assigned to a local
 * and passed as `first_person_weapon_draw`'s argument; both are void/no-argument per their own
 * (already-reconstructed) definitions — this is the session's now-familiar "leftover register value
 * misread as a phantom return/argument" pattern, so both calls are reproduced with no return value/args. */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/blam_data_globals.h"


extern int16_t local_player_count(void);
extern void hcex_prepare_halo_render(int camera_index);
extern void first_person_weapon_render_update(void);
extern void first_person_weapon_draw(void);
extern void *memcpy(void *destination, const void *source, unsigned int size);

void first_person_weapon_update_no_rend(void)
{
    if ( !debug_no_drawing || local_player_count() < 1 )
        return;

    hcex_prepare_halo_render(0);
    first_person_weapon_render_update();
    hcex_fp_model1[0] = 0;
    hcex_fp_model1[1] = 0;
    hcex_fp_model2[1] = 0;
    hcex_fp_model2[0] = 0;
    hcex_model_rend_disable = 1;
    first_person_weapon_draw();
    hcex_model_rend_disable = 0;

    if ( hcex_coop_local_player_index == 1 )
    {
        hcex_fp_model1[0] = hcex_fp_model1[1];
        hcex_fp_model2[0] = hcex_fp_model2[1];
        hcex_fp_model1[1] = 0;
        hcex_fp_model2[1] = 0;
        memcpy(hcex_fp_model_matrices1[0], hcex_fp_model_matrices1[1], 0xD00u);
        memcpy(hcex_fp_model_matrices2[0], hcex_fp_model_matrices2[1], 0xD00u);
    }

    if ( local_player_count() > 1 && hcex_coop_local_player_index < 0 )
    {
        hcex_prepare_halo_render(1);
        first_person_weapon_render_update();
        hcex_model_rend_disable = 1;
        first_person_weapon_draw();
        hcex_model_rend_disable = 0;
    }
}
