/* flush_batch @0x83786638 */
/* flush_batch 0x83786638 — draw and reset one accumulated widget batch. When the batch has vertices and its
 * cached render state can be applied (set_state), draw its triangles out of the batch's inline vertex buffer
 * (28-byte dynamic_unlit_vertex_spec stride), then clear the fill cursor and time stamp. */

#include <stdint.h>
#include "headers/widget_batch.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


#include "headers/widget_state.h"
extern uint8_t set_state(widget_state *new_state);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
                                     const void *vertex_data, unsigned int vertex_stride);

void flush_batch(int batch_index)
{
    if ( widget_batches[batch_index].vertex_index && set_state(&widget_batches[batch_index].state) )
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLELIST,
                                 3 * (widget_batches[batch_index].vertex_index / 3),
                                 &widget_batches[batch_index], 0x1C);

    widget_batches[batch_index].vertex_index = 0;
    widget_batches[batch_index].time_stamp = 0;
}
