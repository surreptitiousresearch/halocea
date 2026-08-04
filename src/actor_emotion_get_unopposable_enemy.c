/* actor_emotion_get_unopposable_enemy @0x837D8128 — finds the existing actor_unopposable_enemy record for
 * `unit_index` in `enemies` (linear search keyed by record.unit_index), or creates one (default-initialized:
 * unopposable_type=0, prop_index=-1, prop=nullptr, friends_retreating=0, closest_retreating_friend_dist_sq=
 * FLT_MAX, closest_retreating_friend_actor_index=-1) if there's room under `maximum_enemy_count` and bumps *enemy_count.
 * Returns the record's index, or -1 if not found and no room to create one.
 *
 * DEVIATION: the DB's own applied prototype names 5 parameters (actor_index, unit_index, enemy_count*,
 * maximum_enemy_count, enemies*), but the disasm proves only FOUR register args are read-before-write:
 * r3 = int unit_index (saved into r7 at entry, then compared against each record.unit_index at +8),
 * r4 = int16_t *enemy_count (read once, written back as count+1 on create), r5 = int16_t maximum_enemy_count
 * VALUE (not a pointer), r6 = actor_unopposable_enemy *enemies array base (used in `28*i + enemies`).
 * The DB's 5th "enemies" slot (r7) does NOT exist — r7 is just a copy of r3(unit_index); there is no 5th
 * argument. Reconstructed positionally from disasm rather than trusting the applied signature. */

#include <stdint.h>
#include "headers/actor_unopposable_enemy.h"
#include "headers/actor_unopposable_danger_type.h"

int16_t actor_emotion_get_unopposable_enemy(int unit_index, int16_t *enemy_count, int16_t maximum_enemy_count,
        actor_unopposable_enemy *enemies)
{
    int16_t count = *enemy_count;

    int16_t found_index = -1;
    if (count > 0)
    {
        for (int16_t i = 0; i < count; i++)
        {
            if (enemies[i].unit_index == unit_index)
            {
                found_index = i;
                break;
            }
        }
    }

    if (found_index != -1 || count >= maximum_enemy_count)
        return found_index;

    *enemy_count = count + 1;
    actor_unopposable_enemy *record = &enemies[count];
    record->unopposable_type = _actor_unopposable_danger_none;
    record->closest_retreating_friend_dist_sq = 3.4028235e38f;
    record->prop_index = -1;
    record->unit_index = -1;
    record->prop = nullptr;
    record->friends_retreating = 0;
    record->closest_retreating_friend_actor_index = -1;
    return count;
}
