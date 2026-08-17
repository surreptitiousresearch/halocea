/* cache_files_dispose @0x83754E10 */
#include <stdint.h>
#include "headers/cache_file_globals.h"
#include "headers/blam_data_globals.h"


extern void cache_file_close(void);
extern uint8_t data_file_close(unsigned int data_file_id);
extern void dlFree(void *ptr);

void cache_files_dispose(void)
{
    if ((uint16_t)cache_file_globals_0.open_map_file_index != 0xFFFF)
        cache_file_close();

    data_file_close(1u);
    dlFree(cache_file_globals_0.requests);
}
