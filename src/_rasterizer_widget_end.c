/* _rasterizer_widget_end @0x83786F68 */
/* _rasterizer_widget_end 0x83786F68 — finish the UI-widget pass: flush every batch that has
 * accumulated vertices, then end and release the widget DX effect.
 *
 * DEVIATION: the decompiler walked the batches by pointer (p += 21510 uints) and rendered the
 * loop bound as the coincidental adjacent symbol &sound_channels[165]; that constant is
 * widget_batches_base + 5*sizeof(widget_batch) (stride 86040 = the 21510-uint step), i.e. the
 * end of the 5-element widget_batches[] array. Restored to an indexed loop. */

#include "headers/widget_batch.h"
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/blam_data_globals.h"


extern void flush_batch(int batch_index);

void _rasterizer_widget_end(void)
{
    int batch_index;

    for ( batch_index = 0; batch_index < 5; ++batch_index )
    {
        if ( widget_batches[batch_index].vertex_index )
            flush_batch(batch_index);
    }

    if ( widget_dxeffect_shader && widget_dxeffect_shader->effect )
    {
        widget_dxeffect_shader->effect->lpVtbl->EndPass(widget_dxeffect_shader->effect);
        widget_dxeffect_shader->effect->lpVtbl->End(widget_dxeffect_shader->effect);
    }
    widget_dxeffect_shader = 0;
}
