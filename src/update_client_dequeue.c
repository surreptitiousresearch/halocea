/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* update_client_dequeue @ 0x836F65B8 — pull the next numbered network update off the client queue and
 * turn it into this tick's per-player action array. It returns 0 (and does nothing) if the next update
 * hasn't arrived yet. Otherwise it first scatters the update's packed actions into the per-player queue
 * data array, then reads each queue entry back into the caller's `actions` array (computing the effective
 * control flags as held & ~latched, then re-latching the sticky bits) and reports each player's completed
 * client-update id. Finally it advances the dequeue counter. Returns 1 on success.
 *
 * The per-player queue datum is an unmodeled record laid out like a player_action with a header; its
 * fields are reached by raw int/float/word index, annotated inline. */

#include <stdint.h>
#include "headers/update_client_globals.h"
#include "headers/player_action.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"

extern update *update_client_get_update(int update_number);

uint8_t update_client_dequeue(player_action *actions, int *completed_client_update_ids)
{
    update *update;
    data_iterator iter;
    int index;

    update = update_client_get_update(update_client_globals.next_update_number_to_dequeue);
    if ( !update
      || update_client_globals.next_update_number_to_dequeue > update_client_globals.latest_update_number_received )
        return 0;

    /* scatter the update's packed actions into the per-player queue data array */
    data_iterator_new(&iter, update_client_globals.queues);
    index = -1;
    for ( float *queue_action = data_iterator_next(&iter);
          queue_action;
          queue_action = data_iterator_next(&iter) )
    {
        index = (int16_t)(index + 1);
        if ( index < update->data.number_of_actions )
        {
            /* The source is NOT an unmodeled wire record: `(char *)&update->update_number +
             * 32*index` is `&update->data.actions[index] - 8`, because actions[] starts 8 bytes
             * into `update` (update_number@0 + number_of_actions@4 + pad@6). The decompiler's
             * float cursor then reached the fields as src[2..7] / src_words[16..18], which are
             * exactly player_action's members. The 32 was sizeof(player_action).
             * The DESTINATION queue datum is still unmodeled — a per-player record with a header
             * slot, whose fields stay index-addressed and are annotated inline. */
            const player_action *src = &update->data.actions[index];
            /* DEVIATION: slot [1] holds the control flags, which the read-back loop below consumes
             * as an int (`queue_action[1] & ~queue_action[2]`); the decompiler copied them through
             * a float lvalue only because it had typed the cursor float*. Copied as an int. */
            ((int *)queue_action)[1] = src->control_flags;
            queue_action[3] = src->desired_facing.n[0];
            queue_action[4] = src->desired_facing.n[1];
            queue_action[5] = src->throttle.n[0];
            queue_action[6] = src->throttle.n[1];
            queue_action[7] = src->primary_trigger;
            uint16_t *queue_action_words = (uint16_t *)queue_action;
            queue_action_words[16] = src->desired_weapon_index;
            queue_action_words[17] = src->desired_grenade_index;
            queue_action_words[18] = src->desired_zoom_level;
        }
    }

    /* read each queue entry back out into the caller's action array */
    data_iterator_new(&iter, update_client_globals.queues);
    index = -1;
    for ( int *queue_action = data_iterator_next(&iter);
          queue_action;
          queue_action = data_iterator_next(&iter) )
    {
        player_action *out;
        index = (int16_t)(index + 1);
        out = &actions[index];

        out->control_flags = queue_action[1] & ~queue_action[2];   /* held & ~latched */
        queue_action[2] = queue_action[1] & 0x4D0;                 /* re-latch the sticky bits */
        out->desired_facing.n[0] = ((float *)queue_action)[3];
        out->desired_facing.n[1] = ((float *)queue_action)[4];
        out->throttle.n[0] = ((float *)queue_action)[5];
        out->throttle.n[1] = ((float *)queue_action)[6];
        out->primary_trigger = ((float *)queue_action)[7];
        uint16_t *queue_action_words = (uint16_t *)queue_action;
        out->desired_weapon_index = queue_action_words[16];
        out->desired_grenade_index = queue_action_words[17];
        out->desired_zoom_level = queue_action_words[18];
        completed_client_update_ids[index] = update->data.completed_client_update_id[index];
    }
    ++update_client_globals.next_update_number_to_dequeue;

    return 1;
}
