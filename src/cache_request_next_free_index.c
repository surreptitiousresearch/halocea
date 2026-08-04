#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"


int cache_request_next_free_index(void)
{
    for ( ;; )
    {
        int index = 0;

        while ( cache_file_globals_0.requests[index].pending )
        {
            if ( ++index >= 512 )
                break;
        }
        if ( index < 512 )
            return index;
    }
}
