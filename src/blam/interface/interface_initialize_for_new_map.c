/* interface_initialize_for_new_map @0x836A7548 — bring up the HUD, text drawing and first-person weapon
 * interfaces for a new map, then set the default text-draw font from the interface tag references. */

#include <stdint.h>
#include "headers/game_globals_tag.h"
#include "headers/text_justification.h"
#include "headers/tag_reference.h"
#include "headers/real_argb_color.h"
#include "headers/interface_tag_index.h"
#include "headers/blam_data_globals.h"

#include "headers/real_argb_color.h"
extern void hud_initialize_for_new_map(void);
extern void draw_string_initialize_for_new_map(void);
extern void first_person_weapons_initialize_for_new_map(void);
extern void draw_string_set_draw_mode(int font_index, int16_t style, int16_t justification, unsigned int flags, const real_argb_color *color);

void interface_initialize_for_new_map(void)
{
    hud_initialize_for_new_map();
    draw_string_initialize_for_new_map();
    first_person_weapons_initialize_for_new_map();

    tag_reference *interface_references;
    if ( global_game_globals->interface_tag_references.count )
        interface_references = (tag_reference *)global_game_globals->interface_tag_references.address;
    else
        interface_references = nullptr;

    /* default terminal font tag index (interface_tag_references slot _interface_font_terminal) */
    draw_string_set_draw_mode(interface_references[_interface_font_terminal].index, -1, _text_justification_left, 0,
                              global_real_argb_white);
}
