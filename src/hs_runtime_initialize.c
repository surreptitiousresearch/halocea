/* hs_runtime_initialize @ 0x8368CAA0 — HaloScript thread/global pools; pre-create
 * one global datum per external global. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
#include "headers/data_array.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern void data_make_valid(data_array *data);
extern int datum_new_at_index(data_array *data, int index);
extern short hs_external_global_count;

void hs_runtime_initialize(void)
{
    hs_thread_data = game_state_data_new("hs thread", 256, 536);
    hs_global_data = game_state_data_new("hs globals", 1024, 8);
    if ( hs_thread_data && hs_global_data )
    {
        int i;
        data_make_valid(hs_global_data);
        for ( i = 0; i < hs_external_global_count; ++i )
            datum_new_at_index(hs_global_data, i | 0xACED0000);  /* salted datum id */
    }
}
