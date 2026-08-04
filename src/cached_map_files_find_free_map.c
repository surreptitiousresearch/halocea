/* cached_map_files_find_free_map @0x83754BC0 — choose a cached-map slot to (re)use for a map of the given size
 * and scenario_type. scenario_type selects the candidate slot range; among free slots (not the currently open
 * one) whose capacity class can hold `size`, prefer the smallest sufficient capacity class, tie-breaking on
 * the oldest last-modification time. Returns the slot index, or -1 if none fits.
 *
 * Slot capacity classes: index<=1 -> 0x18000000, index==2 -> 0x2300000, index>2 -> 0x8000000.
 *
 * Deviation: for scenario_type >= NUMBER_OF_SCENARIO_TYPES the decompiler reads an uninitialized stack slot (HIWORD(v14)) as the slot
 * range — a degenerate/unused path reproduced as a read of an indeterminate value; left as-is below. */

/* removed <windows.h>: canonical blam_data_globals.h provides self-contained Win32 type shims (tagRECT/HWND__/_OVERLAPPED/_FILETIME); system windows.h collided. Win32 fn protos resolve via project headers/implicit-decl. */
#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/scenario_type.h"
#include "headers/blam_data_globals.h"
#include "headers/_FILETIME.h"

/* Win32 boundary function shim (windows.h intentionally excluded) */
extern long __stdcall CompareFileTime(const FILETIME *, const FILETIME *);


static int slot_capacity(int index)
{
    if ( index > 1 )
        return index > 2 ? 0x8000000 : 36700160;
    return 402653184;
}

int16_t cached_map_files_find_free_map(int size, int16_t scenario_type)
{
    int low, high;

    if ( scenario_type )
    {
        if ( scenario_type == _scenario_type_multiplayer )
        {
            low = 3; high = 5;
        }
        else if ( (unsigned int)scenario_type >= NUMBER_OF_SCENARIO_TYPES )
        {
            int indeterminate;   /* deviation: uninitialized in the original */
            low = indeterminate; high = indeterminate;
        }
        else
        {
            low = 2; high = 2;
        }
    }
    else
    {
        low = 0; high = 1;
    }

    int chosen_index = -1;
    cached_map_file *best_file = 0;

    if ( (short)low <= high )
    {
        for ( int index = (short)low; index <= high; index = (short)(index + 1) )
        {
            if ( cache_file_globals_0.open_map_file_index == index )
                continue;
            if ( slot_capacity(index) <= size )
                continue;

            if ( chosen_index == -1
                 || slot_capacity(index) < slot_capacity((short)chosen_index)
                 || CompareFileTime(&best_file->last_modification_date,
                                    &cache_file_globals_0.cached_map_files[index].last_modification_date) > 0 )
            {
                chosen_index = index;
                best_file = &cache_file_globals_0.cached_map_files[index];
            }
        }
    }

    return chosen_index;
}
