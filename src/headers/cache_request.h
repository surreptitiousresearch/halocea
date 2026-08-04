#ifndef CACHE_REQUEST_H
#define CACHE_REQUEST_H

#include <stdint.h>
#include "_OVERLAPPED.h"
#include "cache_file_read_request_params.h"

typedef struct cache_request
{
    _OVERLAPPED                     overlapped;
    int32_t                         size;
    void                           *buffer;
    uint8_t                         blocking;
    uint8_t                         pending;
    uint8_t                         running;
    uint8_t                         pad[1];
    uint8_t                         data_file;
    uint8_t                         _pad[3];
    cache_file_read_request_params  params;
} cache_request;

#endif /* CACHE_REQUEST_H */
