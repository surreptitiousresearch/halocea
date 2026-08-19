/* get_flash_duration @0x8379E5D8 */
#include "headers/hud_color_definition.h"

int get_flash_duration(const hud_color_definition *hud_color)
{
    return (int)(hud_color->flash_period * 30.0f);
}
