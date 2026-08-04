#ifndef CACHED_MAP_FILE_H
#define CACHED_MAP_FILE_H

#include "cache_file_header.h"
#include "_FILETIME.h"

typedef struct cached_map_file
{
    void              *handle;
    _FILETIME          last_modification_date;
    cache_file_header  header;
} cached_map_file;

#endif /* CACHED_MAP_FILE_H */
