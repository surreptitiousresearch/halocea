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
            /* source action[index]: update + 32*index bytes; floats [2..7] = facing/throttle/trigger,
             * words [16..18] = weapon/grenade/zoom.
             * typerec residue: packed wire-format records (no DB type; the 32-byte on-the-wire action
             * and the per-player queue datum are call-site-local layouts, kept byte-exact). */
            float *src = (float *)((char *)&update->update_number + 32 * index);
            queue_action[1] = src[2];
            queue_action[3] = src[3];
            queue_action[4] = src[4];
            queue_action[5] = src[5];
            queue_action[6] = src[6];
            queue_action[7] = src[7];
            /* words [16..18] = desired weapon/grenade/zoom (packed wire; no DB type — word cursors) */
            uint16_t *queue_action_words = (uint16_t *)queue_action;
            uint16_t *src_words = (uint16_t *)src;
            queue_action_words[16] = src_words[16];
            queue_action_words[17] = src_words[17];
            queue_action_words[18] = src_words[18];
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
