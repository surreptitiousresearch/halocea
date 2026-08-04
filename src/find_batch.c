/* find_batch @0x83786788 — find (or make room for) the widget_batches[] slot matching the pending
 * widget_state_new, for the UI widget renderer to accumulate vertices into. Scans all 5 batches for one
 * whose cached state exactly matches widget_state_new; if found, bumps its time stamp and returns it
 * directly. Otherwise evicts the least-recently-used batch (lowest time_stamp): flushes it first if it has
 * pending vertices (same draw as flush_batch.c), then re-keys it to widget_state_new and returns it.
 *
 * DEVIATION: the decompiler rendered the per-batch state comparison as a manually-unrolled byte-by-byte
 * walk (`HIBYTE(widget_state_new.stage_index)` against successive bytes of the batch's cached state) — this
 * is exactly a `memcmp(&batch.state, &widget_state_new, sizeof(widget_state)) == 0`, restored here as a
 * plain field comparison. widget_state has a 2-byte pad after stencil_mode that the raw byte walk also
 * compares but this rewrite doesn't; harmless in practice since every write to a batch's `state` (here and
 * in _rasterizer_widget_begin.c) is a whole-struct assignment from widget_state_new, keeping the pad bytes
 * identical wherever a match is possible. The loop's upper bound
 * (`&sound_channels[165].queued_permutation`) is the same coincidental adjacent-global artifact already
 * documented in flush_all_batches.c, equal to `&widget_batches[5]`. */

#include <stdint.h>
#include "headers/widget_batch.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"


#include "headers/widget_state.h"
extern uint8_t set_state(widget_state *new_state);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
                                     const void *vertex_data, unsigned int vertex_stride);

int find_batch(void)
{
    int lru_index = 0;
    unsigned int lru_time_stamp = widget_batches[0].time_stamp;
    int batch_index;

    for ( batch_index = 0; batch_index < 5; ++batch_index )
    {
        if ( widget_batches[batch_index].state.stage_index == widget_state_new.stage_index
          && widget_batches[batch_index].state.bitmap_group_index == widget_state_new.bitmap_group_index
          && widget_batches[batch_index].state.sequence_index == widget_state_new.sequence_index
          && widget_batches[batch_index].state.stencil_mode == widget_state_new.stencil_mode )
        {
            break;
        }

        if ( widget_batches[batch_index].time_stamp < lru_time_stamp )
        {
            lru_time_stamp = widget_batches[batch_index].time_stamp;
            lru_index = batch_index;
        }
    }

    if ( batch_index < 5 )
    {
        widget_batches[batch_index].time_stamp = ++widget_time_stamp;
        return batch_index;
    }

    if ( widget_batches[lru_index].vertex_index )
    {
        if ( set_state(&widget_batches[lru_index].state) )
            D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_TRIANGLELIST,
                                     3 * (widget_batches[lru_index].vertex_index / 3),
                                     &widget_batches[lru_index], 0x1Cu);
    }

    widget_batches[lru_index].vertex_index = 0;
    widget_batches[lru_index].state = widget_state_new;
    widget_time_stamp = widget_time_stamp + 1;
    widget_batches[lru_index].time_stamp = widget_time_stamp;

    return lru_index;
}
