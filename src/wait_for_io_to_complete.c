/* wait_for_io_to_complete @0x83808460 — block (up to 11 * 10-second alertable sleeps) until every
 * in-flight overlapped read/write buffer finishes, then clear the completed-flags word. This build only
 * has one buffer slot to check (the loop bound is 1), so `overlapped_in_use_flags` (a bitmask per the
 * header, but read here via 4-byte array-element addressing matching the compiled `self + 0x998 + 4*i`) is
 * only ever tested at index 0. */

#include <stdint.h>
#include "headers/simple_decompressor_definition.h"

extern int SleepEx(unsigned int milliseconds, uint8_t alertable);

void wait_for_io_to_complete(simple_decompressor_definition *self)
{
    int retries_left = 11;

    while ( 1 )
    {
        uint8_t still_pending = 0;
        for ( int i = 0; i < 1; ++i )
        {
            if ( self->overlapped_in_use_flags[i] )
                still_pending = 1;
        }

        if ( !still_pending )
            break;
        if ( retries_left-- == 0 )
            break;

        SleepEx(10000, 1);
    }

    self->overlapped_completed_flags[0] = 0;
}
