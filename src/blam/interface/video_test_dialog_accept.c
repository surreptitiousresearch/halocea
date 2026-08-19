/* video_test_dialog_accept @0x837854F0 — "accept" handler for the video test dialog; always confirms.
 * `widget`/`event`/`widget_deleted` are unused. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"


uint8_t video_test_dialog_accept(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    video_test_save_confirmed = 1;
    return 1;
}
