/* ui_widget_game_data_function_invoke @0x8377E190 — dispatch a widget's "game data" function-list index
 * (bit 0x8000 reserved/unset, indices 0..0x2A valid) to its handler. */

#include <stdint.h>
#include "headers/widget_instance.h"

extern void (*game_data_input_function_list[0x2B])(widget_instance *widget);

/* attested: caller (widget_instance_render_recursive @0x83734F18) passes r4 via lhzx with no
 * sign-extension — declared param is uint16_t; the callee's extsh @0x8377E190 is the source-level
 * (int16_t) sign check below, the clrlwi-16 @0x8377E19C is u16-param canonicalization. */
void ui_widget_game_data_function_invoke(widget_instance *widget, uint16_t function)
{
    if ( (int16_t)function >= 0 && function < 0x2Bu )
        game_data_input_function_list[(int16_t)function](widget);
}
