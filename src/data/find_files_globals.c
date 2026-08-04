/* find_files_globals @ 0x84184D38, 616 bytes (data-init).
   Recursive directory-walk state (find_files.c). Reconstructed from the raw image
   (big-endian). Non-zero fields:
     - depth   = -1  (idle sentinel, __int16 at 0x04)
     - handles[0..7] = INVALID_HANDLE_VALUE (0xFFFFFFFF) at 0x108..0x124
   All other fields (flags, location, path[256], data) are zero-filled. */
#include "../headers/find_files_globals.h"

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE ((void *)(long)-1)
#endif

find_files_globals_t find_files_globals = {
    .depth   = -1,
    .handles = {
        INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
        INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
        INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
        INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
    },
};
