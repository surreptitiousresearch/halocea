/* player_profile_3wide_list_update @0x83781948 — tail-call forwarder
 * (`b player_profile_single_preview_column_list_update`; confirmed via disasm, not a bare blr). The
 * "3-wide" profile list variant collapsed to the single-preview-column list's update routine.
 * player_profile_single_preview_column_list_update itself is not yet decompiled — declared extern
 * with its own DB prototype and called through with the same argument. */

#include "headers/widget_instance.h"

extern void player_profile_single_preview_column_list_update(widget_instance *widget);

void player_profile_3wide_list_update(widget_instance *list_widget)
{
    player_profile_single_preview_column_list_update(list_widget);
}
