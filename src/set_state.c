/* set_state 0x83786560 — apply a cached widget render state (texture stage + stencil mode) to the rasterizer,
 * skipping the work when it already matches the current state. Returns whether the texture is ready (the
 * non-blocking texture set did not stall); the stencil mode is always reconciled. Caller (flush_batch) treats
 * the boolean result as the gate for issuing the widget batch draw.
 *
 * The return is the boolean v3 (0/1); declared unsigned __int8 to match the existing flush_batch caller. */

#include <stdint.h>
#include "headers/widget_state.h"
#include "headers/blam_data_globals.h"


extern int rasterizer_set_texture_non_blocking(int16_t stage, int16_t type, int16_t usage, int bitmap_group_index, int16_t bitmap_index);
extern uint8_t rasterizer_set_texture_direct_non_blocking(int16_t stage, int bitmap_group_index, int16_t bitmap_index);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);

uint8_t set_state(widget_state *new_state)
{
    int stage_index = new_state->stage_index;
    unsigned __int8 succeeded = 1;
    if ( widget_state_current.stage_index != new_state->stage_index
      || widget_state_current.bitmap_group_index != new_state->bitmap_group_index
      || widget_state_current.sequence_index != new_state->sequence_index )
    {
        unsigned __int8 stalled;
        if ( new_state->bitmap_group_index == -1 )
            stalled = rasterizer_set_texture_direct_non_blocking(stage_index, -1, new_state->sequence_index);
        else
            stalled = rasterizer_set_texture_non_blocking(stage_index, 0, 1, new_state->bitmap_group_index,
                                                          new_state->sequence_index);
        widget_state_current.stage_index = new_state->stage_index;
        succeeded = stalled == 0;
        widget_state_current.bitmap_group_index = new_state->bitmap_group_index;
        widget_state_current.sequence_index = new_state->sequence_index;
    }

    if ( widget_state_current.stencil_mode != new_state->stencil_mode )
    {
        rasterizer_set_stencil_mode(new_state->stencil_mode);
        widget_state_current.stencil_mode = new_state->stencil_mode;
    }
    return succeeded;
}
