/* hcex_is_anitec_render_mode @ 0x823D1628
   Returns whether the "anitec" animation-capture render mode is active: true
   when the app system exists and has at least one registered anitec name. */

#include "../headers/gs_app_system_boundary.h"

extern "C" int hcex_is_anitec_render_mode(void)
{
    if ( !gsAppSystem )
        return 0;
    if ( !gsAppSystem->anitecNames.nElem )
        return 0;
    return 1;
}
