#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/blam_data_globals.h"


uint8_t cancel_profile_delete(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    delete_profile_index = -1;
    delete_savegame_filename[0] = 0;
    return 1;
}
