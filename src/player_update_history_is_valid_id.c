#include <stdint.h>
/* player_update_history_is_valid_id @0x837ABC50 — true if update_id is a valid index into the
 * 64-entry player update history (0 <= update_id < 64). */

uint8_t player_update_history_is_valid_id(int update_id)
{
    if (update_id < 0)
        return 0;
    if (update_id >= 64)
        return 0;
    return 1;
}
