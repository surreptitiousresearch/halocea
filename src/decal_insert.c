/* decal_insert @0x83741420 — allocate a decal at a given cache slot and splice it into a cluster/layer's decal
 * list. A permanent decal is marked type 2 and bumps the permanent count. A transient decal is (roughly 10% of
 * the time, by RNG) marked "locked" (type 1) and counts against a locked budget; when the locked budget exceeds
 * 512 it walks the decal pool — restarting up to 100 times — probabilistically un-locking existing locked decals
 * until the count falls back to 256. The new decal is then linked: with next_decal_index == -1 it is reinserted
 * via decal_reinsert; otherwise it is spliced ahead of next_decal in the doubly-linked list, updating the
 * layer/cluster head when it becomes the first entry.
 *
 * RNG thresholds (655350, 2686935) are the shipped literals (100 * a u16 seed sample). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/decal_datum.h"
#include "headers/decal_flags.h"
#include "headers/data_iterator.h"
#include "headers/decal_globals.h"
#include "headers/blam_data_globals.h"


extern int datum_new_at_index(data_array *data, int index);
extern uint32_t *get_global_local_random_seed_address(void);
extern uint16_t seed_random(uint32_t *seed);
extern void decal_reinsert(int decal_index, int16_t cluster_index, int16_t layer);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

int decal_insert(int cache_index, int16_t cluster_index, int16_t layer, int next_decal_index,
        uint8_t permanent)
{
    int decal_index = datum_new_at_index(global_decal_data, cache_index);
    if ( decal_index == -1 )
        return -1;

    decal_datum *new_decal = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, decal_index);

    if ( permanent )
    {
        new_decal->flags = 1u << _decal_permanent_bit;
        ++decal_globals->permanent_count;
    }
    else
    {
        unsigned int *seed = get_global_local_random_seed_address();
        unsigned __int16 roll = seed_random(seed);
        if ( 100 * roll >= 655350 )
        {
            new_decal->flags = 0;
        }
        else
        {
            new_decal->flags = 1u << _decal_locked_bit;
            int locked_count = decal_globals->locked_count + 1;
            decal_globals->locked_count = locked_count;
            if ( locked_count > 512 )
            {
                int restart_count = 0;
                data_iterator iterator;
                data_iterator_new(&iterator, global_decal_data);
                if ( decal_globals->locked_count > 256 )
                {
                    for ( ;; )
                    {
                        decal_datum *decal = data_iterator_next(&iterator);
                        if ( !decal )
                        {
                            data_iterator_new(&iterator, global_decal_data);
                            ++restart_count;
                            if ( restart_count >= 100 )
                                return -1;
                        }
                        else if ( (decal->flags & (1u << _decal_locked_bit)) != 0 )   /* locked */
                        {
                            unsigned int *evict_seed = get_global_local_random_seed_address();
                            if ( !(100 * seed_random(evict_seed) >= 2686935
                                    && (unsigned __int16)decal->cluster_index != 0xFFFF) )
                            {
                                decal->flags &= ~(1u << _decal_locked_bit);
                                --decal_globals->locked_count;
                            }
                        }

                        if ( decal_globals->locked_count <= 256 )
                            break;
                    }
                }
            }
        }
    }

    if ( next_decal_index == -1 )
    {
        decal_reinsert(decal_index, cluster_index, layer);
        return decal_index;
    }

    decal_datum *next_decal = DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, next_decal_index);
    if ( next_decal->prev_decal_index == -1 )
        decal_globals->first_decal_indices[layer][cluster_index] = decal_index;
    else
        DATA_ARRAY_ELEMENT(global_decal_data, decal_datum, next_decal->prev_decal_index)->next_decal_index = decal_index;

    next_decal->prev_decal_index = decal_index;
    new_decal->prev_decal_index = decal_index;
    new_decal->next_decal_index = next_decal_index;
    new_decal->cluster_index = cluster_index;
    new_decal->layer = layer;
    return decal_index;
}
