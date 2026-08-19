/* checking_for_updates_update @ 0x836984B0
   Per-frame update for the "checking for updates" UI screen. While the screen
   is not in its closing state, resets the header text box's string index and
   dims the spinner child widget (found two siblings down, then one child in)
   so it never receives events and renders at reduced alpha. */

#include "headers/widget_instance.h"

/* Loaded via lbz @0x836984CC — a single-byte bool flag (DB-typeless global). */
extern unsigned char checking_for_updates_closing;

void checking_for_updates_update(widget_instance *widget)
{
    widget_instance *header = widget->children;
    widget_instance *spinner = header->next->next->children;

    if ( !checking_for_updates_closing )
    {
        header->parameters.text_box_parameters.string_list_index = 0;
        spinner->never_receive_events = 1;
        spinner->alpha_modifier = 0.333f;
    }
}
