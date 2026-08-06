/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
#ifndef CACHE_FILE_GLOBALS_H
#define CACHE_FILE_GLOBALS_H

#include <stdint.h>
#include "cached_map_file.h"
#include "cache_request.h"

typedef struct cache_file_globals_s
{
    cached_map_file  cached_map_files[6];
    uint8_t          copy_in_progress;
    uint8_t          _pad1[1];
    int16_t          copying_to_map_file_index;
    char             copying_to_map_file_name[32];
    int16_t          open_map_file_index;
    int16_t          blocking_request_index;
    void            *sleep_event;
    void            *thread;
    cache_request   *requests;
} cache_file_globals_s;

#ifdef __cplusplus
extern "C" {
#endif

extern cache_file_globals_s cache_file_globals_0;

#ifdef __cplusplus
}
#endif

#endif /* CACHE_FILE_GLOBALS_H */
