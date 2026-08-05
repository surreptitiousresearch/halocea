/* cache_file_block_until_not_busy @0x837543B0 — spin-waits (yielding via SleepEx each pass) until none of
 * the cache file system's 512 request slots are still pending. SleepEx's status is left in r3 at the blr
 * but never computed for return and never consumed — attested void. */

#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"

extern unsigned int SleepEx(unsigned int milliseconds, int alertable);

void cache_file_block_until_not_busy(void)
{
    unsigned char any_pending;

    do
    {
        SleepEx(0, 1);
        any_pending = 0;

        for ( int i = 0; i < 512; ++i )
        {
            if ( cache_file_globals_0.requests[i].pending )
                any_pending = 1;
        }
    }
    while ( any_pending );
}
