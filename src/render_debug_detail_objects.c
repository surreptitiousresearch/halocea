/* render_debug_detail_objects @0x837116A8 — debug tool: while single-player, a local player is actively
 * rendering, and `debug_detail_objects` is enabled, nudge every detail-object cell's Z reference vector's
 * W component by an eighth of the accumulated `fudge_offset` (once per frame the pulse is "active", per
 * `fudge_vector`), for player 0's current view. Resets `fudge_offset` to `final_offset` and clears
 * `fudge_vector` afterward, priming the next pulse. Returns whatever `local_player_count()` returned. */

#include <stdint.h>
#include "headers/detail_object_global_runtime_data.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


extern int16_t local_player_count(void);

int render_debug_detail_objects(void)
{
    __int16 result = local_player_count();

    if ( result == 1 && (unsigned __int16)render.local_player_index != 0xFFFF && debug_detail_objects )
    {
        detail_object_runtime_data *view = &detail_object_global_runtime_data->local_player_data[0];

        if ( view->view_data.layer_count > 0 )
        {
            unsigned __int8 pulse_active = fudge_vector;
            float offset = fudge_offset;

            for ( __int16 layer_index = 0; layer_index < view->view_data.layer_count; layer_index++ )
            {
                detail_object_layer_data *layer = &view->layers[layer_index];

                for ( __int16 cell_index = 0; cell_index < layer->cell_count; cell_index++ )
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

    return result;
}
