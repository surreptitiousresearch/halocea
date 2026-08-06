#ifndef ERROR_GLOBAL_DATA_H
#define ERROR_GLOBAL_DATA_H

#include <stdint.h>

typedef struct error_global_data
{
    uint8_t  delayed;
    uint8_t  output_to_debug_file;
    uint8_t  display_state;
    uint8_t  recursion_lock;
    uint8_t  overflow_suppression;
    uint8_t  suppress_all;
    uint8_t  developer_mode;
    uint8_t  _pad;
    int16_t  message_buffer_size;
    char     message_buffer[4096];
} error_global_data;

#ifdef __cplusplus
extern "C" {
#endif

extern error_global_data error_globals;

#ifdef __cplusplus
}
#endif

#endif /* ERROR_GLOBAL_DATA_H */
