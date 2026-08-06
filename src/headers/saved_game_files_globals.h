#pragma once
/* _saved_game_files_globals @ saved_game_files_globals — save-file paths and mutexes
 * (saved_game_files.c). mutex modeled opaque; layout faithful. */

#include <stdint.h>
#include "file_reference.h"

typedef struct mutex mutex;

typedef struct _saved_game_files_globals
{
    file_reference memory_unit_mapfile;          /* 0x000 */
    mutex         *general_mutex;                 /* 0x10C */
    mutex         *mapfile_mutex;                 /* 0x110 */
    int16_t        next_enumerated_profile_index; /* 0x114 */
    unsigned char  initialized;                   /* 0x116 */
    unsigned char  memory_units_dirty;            /* 0x117 */
    unsigned char  enumeration_in_progress;       /* 0x118 */
    char           paths[10][256];                /* 0x119 */
} _saved_game_files_globals;                      /* 2844 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern _saved_game_files_globals saved_game_files_globals;

#ifdef __cplusplus
}
#endif
