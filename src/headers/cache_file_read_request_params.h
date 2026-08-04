#ifndef CACHE_FILE_READ_REQUEST_PARAMS_H
#define CACHE_FILE_READ_REQUEST_PARAMS_H

#include <stdint.h>

typedef struct cache_file_read_request_params cache_file_read_request_params;

struct cache_file_read_request_params
{
    volatile uint8_t                          *finished_flag;
    void (__fastcall *finished_func)(cache_file_read_request_params *);
    void                                      *userdata;
};

#endif /* CACHE_FILE_READ_REQUEST_PARAMS_H */
