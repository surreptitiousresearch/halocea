#include "headers/blam_data_globals.h"
/* walk_stack @0x83811070 — walk the PowerPC back-chain (stack frame chain) starting from the cached
 * `old_ebp`/`walk_up_current_frame` frame pointer pair, validating each link is 4-byte aligned and never points
 * below the previous frame (guards against a corrupt/overflowed chain) before following it. Skips
 * `ignore_levels - 1` frames, then records up to `number_of_levels` return addresses (frame+4, the link
 * register save slot) into `routine_addresses`, stopping early at a null/invalid frame or a zero return
 * address. `*levels_dumped` receives the number of addresses actually recorded. */

extern unsigned int walk_up_current_frame;

static unsigned int *validate_frame(unsigned int candidate, unsigned int *floor)
{
    if ( (candidate & 3) != 0 || candidate < (unsigned int)floor )
        return nullptr;
    return (unsigned int *)candidate;
}

void walk_stack(unsigned int *routine_addresses, unsigned int number_of_levels, unsigned int ignore_levels,
    unsigned int *levels_dumped)
{
    unsigned int *floor = old_ebp;
    unsigned int *frame = validate_frame(walk_up_current_frame, floor);
    walk_up_current_frame = (unsigned int)frame;

    if ( ignore_levels >= 2 )
    {
        for ( unsigned int remaining = ignore_levels - 1; remaining; remaining-- )
        {
            if ( frame )
            {
                frame = validate_frame(*frame, floor);
                walk_up_current_frame = (unsigned int)frame;
                floor = frame;
            }
        }
        old_ebp = floor;
    }

    unsigned int i;
    for ( i = 0; i < number_of_levels; ++routine_addresses )
    {
        unsigned int routine_address = 0;
        if ( frame )
        {
            unsigned int next_frame = *frame;
            routine_address = frame[1];
            frame = validate_frame(next_frame, floor);
            floor = frame;
            walk_up_current_frame = (unsigned int)frame;
            old_ebp = frame;
        }

        *routine_addresses = routine_address;
        if ( !routine_address )
            break;
        ++i;
    }

    *levels_dumped = i;
}
