/* decals_unlock @0x83740598 — clear the lock bit (and, when `permanent` is set, the permanent bit) on every
 * decal, decrementing the corresponding global counts. Decal flags live in the datum's word at +2:
 * bit 0 = locked, bit 1 = permanent. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/data_iterator.h"
#include "headers/decal_globals.h"
#include "headers/decal_datum.h"
#include "headers/decal_flags.h"
#include "headers/blam_data_globals.h"


void decals_unlock(uint8_t permanent)
{
    if ( global_decal_data->valid )
    {
        data_iterator iterator;
        data_iterator_new(&iterator, global_decal_data);
        for ( decal_datum *decal = data_iterator_next(&iterator); decal; decal = data_iterator_next(&iterator) )
        {
            int16_t flags = decal->flags;
            struct decal_globals *globals = decal_globals; /* decal_globals is a bare struct tag */
            if ( (flags & (1u << _decal_locked_bit)) != 0 )
            {
                decal->flags = flags & ~(1u << _decal_locked_bit);
                --globals->locked_count;
            }
            if ( permanent )
            {
                int16_t flags2 = decal->flags;
                if ( (flags2 & (1u << _decal_permanent_bit)) != 0 )
                {
                    decal->flags = flags2 & ~(1u << _decal_permanent_bit);
                    --globals->permanent_count;
                }
            }
        }
    }
}
