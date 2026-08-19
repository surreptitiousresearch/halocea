#include "headers/blam_data_globals.h"
/* walk_stack_context @0x83810F58 — identical body to walk_stack.c (confirmed by decompiler output and by
 * stack_walk_with_context.c's own note that the DB prototype doesn't actually take a context pointer); the
 * "context" variant appears to exist only so callers can express intent, not because it walks differently.
 * See walk_stack.c for the algorithm description. */

extern unsigned int walk_up_current_frame;

static unsigned int *validate_frame(unsigned int candidate, unsigned int *floor)
{
    if ( (candidate & 3) != 0 || candidate < (unsigned int)floor )
        return nullptr;
    return (unsigned int *)candidate;
}

void walk_stack_context(unsigned int *routine_addresses, unsigned int number_of_levels,
    unsigned int ignore_levels, unsigned int *levels_dumped)
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
