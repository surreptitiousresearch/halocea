/* credits_dispose @0x83697F48 */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/dynamic_array.h"
#include "headers/credits_endgame_mode.h"
#include "headers/blam_data_globals.h"

extern void game_end_credits_start(uint8_t in_beginscene);

/* Credits widget teardown handler: frees the credit-line array and, if the credits
 * were rolled at end-of-game (mode > 0), kicks off the post-credits sequence
 * (mode 2 => begin-scene). Returns 1 (event handled). */
uint8_t credits_dispose(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    dynamic_array_delete(&credits_list);
    if (credits_endgame_mode > CREDITS_NOT_ENDGAME)
        game_end_credits_start(credits_endgame_mode == CREDITS_ENDGAME_FINISHED);
    credits_endgame_mode = CREDITS_NOT_ENDGAME;
    return 1;
}
