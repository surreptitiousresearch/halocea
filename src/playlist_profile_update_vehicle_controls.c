/* playlist_profile_update_vehicle_controls @0x83697990 — refresh the six vehicle-slot controls of a playlist
 * profile widget from a packed vehicle set. If the set's top nibble is a preset index (< 8) it is expanded to a
 * full per-vehicle bitfield via game_variant_build_vehicle_set and the controls are dimmed/disabled (this is a
 * read-only preset); otherwise the set is custom and the controls are shown at full alpha and remain interactive.
 * For each of the six slot widgets (the 4th child of the widget and its following siblings) it finds the slot's
 * text-box child and sets its string-list index to the 3-bit vehicle type packed at that slot's position (bits
 * 25-27, 22-24, ... 10-12), clamping any out-of-range type (>= 5) to 0.
 *
 * DEVIATION: vehicle_options_s is a one-int struct; Hex-Rays flattened it to a bare unsigned int for the bit
 * math, so it is accessed via .value here. The six near-identical slot blocks are folded into a loop (slot shift
 * = 25 - 3*i). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/vehicle_options_s.h"
#include "headers/ui_widget_type.h"
#include "headers/game_engine_vehicles.h"

extern uint32_t game_variant_build_vehicle_set(unsigned int preset);

void playlist_profile_update_vehicle_controls(widget_instance *widget, vehicle_options_s vehicle_set)
{
    unsigned int set = vehicle_set.value;
    unsigned __int8 enabled;
    if ( vehicle_set.__s1.preset < _game_engine_vehicles_custom )   /* a read-only built-in preset */
    {
        enabled = 0;
        set = game_variant_build_vehicle_set(vehicle_set.__s1.preset);
    }
    else
    {
        enabled = 1;
    }

    widget_instance *slot = widget->children->next->next->next;
    for ( int i = 0; i < 6; ++i )
    {
        widget_instance *text_box;
        for ( text_box = slot->children; text_box; text_box = text_box->next )
        {
            if ( text_box->widget_type == _ui_widget_type_spinner_list )
                break;
        }

        unsigned int vehicle_type = (set >> (25 - 3 * i)) & 7;
        text_box->parameters.text_box_parameters.string_list_index =
                vehicle_type >= 5 ? 0 : (__int16)vehicle_type;

        if ( enabled )
        {
            slot->alpha_modifier = 1.0f;
            slot->never_receive_events = 0;
        }
        else
        {
            slot->alpha_modifier = 0.333f;
            slot->never_receive_events = 1;
        }

        slot = slot->next;
    }
}
