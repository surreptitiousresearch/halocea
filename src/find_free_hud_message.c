/* find_free_hud_message @0x836A49B0 — pick the HUD message slot to use for a new message. Prefers an invalid
 * slot or one already showing the same (item, offset); failing that, evicts the oldest valid slot. Scans the
 * fixed 4-slot ring. An item_definition_index of -1 matches any slot. */

#include "headers/hud_messaging_datum.h"

hud_message *find_free_hud_message(hud_messaging_datum *datum, int item_definition_index, char message_offset)
{
    hud_message *result = nullptr;
    int oldest_time = 0x7FFFFFFF;
    __int16 oldest_index = 0;

    __int16 i = 0;
    do
    {
        hud_message *message = &datum->messages[i];
        if ((item_definition_index == -1
             || item_definition_index != message->item_definition_index
             || message_offset != message->message_offset)
            && message->valid)
        {
            /* occupied by a different message — remember it as an eviction candidate */
            if (oldest_time > message->time)
            {
                oldest_time = message->time;
                oldest_index = i;
            }
        }
        else
        {
            /* free, or already this message */
            result = &datum->messages[i];
            if (item_definition_index == -1 || item_definition_index == message->item_definition_index)
                break;
        }
        ++i;
    } while ((unsigned __int16)i < 4);

    if (!result)
        return &datum->messages[oldest_index];
    return result;
}
