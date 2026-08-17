/* rasterizer_windows_begin @ 0x836A5D78  (thunk: b _rasterizer_windows_begin)
   Declared return is uint8_t per the byte-normalization rule: all 3 consuming
   callers re-normalize r3 with clrlwi rN,r3,24 right after the call
   (_rasterizer_initialize @0x8369BD24, render_frame_pregame @0x8370764C,
   render_frame @0x83707ABC). */

#include <stdint.h>

extern uint8_t _rasterizer_windows_begin(void);

uint8_t rasterizer_windows_begin(void)
{
    return _rasterizer_windows_begin();
}
