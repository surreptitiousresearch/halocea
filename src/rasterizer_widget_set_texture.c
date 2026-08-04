#include <stdint.h>
/* rasterizer_widget_set_texture @0x836A63B8 — public entry thunk. A single unconditional branch
 * (tail-call) to _rasterizer_widget_set_texture; forwards all arguments unchanged and returns its
 * result. Verified against disasm: `b _rasterizer_widget_set_texture`. */

/* DB prototype of the body @0x837860C8 returns int; thunk's own DB prototype is unsigned __int8. */
extern int _rasterizer_widget_set_texture(int16_t stage_index, int bitmap_group_index,
                                          int16_t sequence_index);

uint8_t rasterizer_widget_set_texture(int16_t stage_index, int bitmap_group_index, int16_t sequence_index)
{
    return _rasterizer_widget_set_texture(stage_index, bitmap_group_index, sequence_index);
}
