/* hs_thread_delete @0x8368D088 */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void hs_thread_delete(int thread_index)
{
    datum_delete(hs_thread_data, thread_index);
}
