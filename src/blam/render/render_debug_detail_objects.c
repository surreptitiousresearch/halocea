/* render_debug_detail_objects @0x837116A8 — debug tool: while single-player, a local player is actively
 * rendering, and `debug_detail_objects` is enabled, nudge every detail-object cell's Z reference vector's
 * W component by an eighth of the accumulated `fudge_offset` (once per frame the pulse is "active", per
 * `fudge_vector`), for player 0's current view. Resets `fudge_offset` to `final_offset` and clears
 * `fudge_vector` afterward, priming the next pulse.
 *
 * DEVIATION (#119): `void`, not `int` — the earlier `return result;` claimed a value the binary does
 * not carry. `lbz r3, fudge_vector(r4)` @0x83711708 overwrites r3 with the pulse byte before the loop
 * (the compiler only allocates the return register to a local when the return value is dead), so on the
 * working path r3 at `blr` @0x837117BC is `fudge_vector`, not `local_player_count`'s result, which is
 * consumed once as `extsh r11, r3` @0x837116B8 and never preserved. The function has no code xrefs
 * (its only xref is the .pdata entry), so nothing consumes r3 on any path. */

#include <stdint.h>
#include "headers/detail_object_global_runtime_data.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


extern int16_t local_player_count(void);

void render_debug_detail_objects(void)
{
    int16_t result = local_player_count();

    if ( result == 1 && (uint16_t)render.local_player_index != 0xFFFF && debug_detail_objects )
    {
        detail_object_runtime_data *view = &detail_object_global_runtime_data->local_player_data[0];

        if ( view->view_data.layer_count > 0 )
        {
            uint8_t pulse_active = fudge_vector;
            float offset = fudge_offset;

            for ( int16_t layer_index = 0; layer_index < view->view_data.layer_count; layer_index++ )
            {
                detail_object_layer_data *layer = &view->layers[layer_index];

                for ( int16_t cell_index = 0; cell_index < layer->cell_count; cell_index++ )
                {
                    if ( pulse_active )
                    {
                        const real_vector4d *z_reference_vector = layer->cells[cell_index].z_reference_vector;
                        ((real_vector4d *)z_reference_vector)->n[3] = offset * 0.125f + z_reference_vector->n[3];
                    }
                }
            }
        }

        fudge_offset = final_offset;
        fudge_vector = 0;
    }
}
