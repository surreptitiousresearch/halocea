/* cache_files_precache_in_progress @0x83753DD8 — true while a background cache copy is running. */
#include <stdint.h>
#include "headers/cache_file_globals.h"
extern cache_file_globals_s cache_file_globals_0;
uint8_t cache_files_precache_in_progress(void)
{
    return cache_file_globals_0.copy_in_progress;
}
