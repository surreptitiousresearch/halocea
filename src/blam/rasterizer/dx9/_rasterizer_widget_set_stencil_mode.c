#include <stdint.h>
#include "headers/widget_state.h"
#include "headers/blam_data_globals.h"


/* DB: this TU is _rasterizer_widget_set_stencil_mode @0x83786150 (impl); the
 * underscore-less public name is the thunk @0x836A63C8 in its own TU. */
void _rasterizer_widget_set_stencil_mode(int16_t stencil_mode)
{
    widget_state_new.stencil_mode = stencil_mode;
}
