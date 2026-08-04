/* player_profile_change_video_settings @0x837852E8 — retail stub: unconditionally returns 1. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/widget_instance.h"

uint8_t player_profile_change_video_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    return 1;
}
