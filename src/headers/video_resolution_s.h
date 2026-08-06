#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* video_resolution_s — one entry in the video_resolutions table. Layout from the database. */

typedef struct video_resolution_s
{
    unsigned int      width;         /* 0x00 */
    unsigned int      height;        /* 0x04 */
    wchar_t           name[16];      /* 0x08 */
    unsigned int      numRefreshes;  /* 0x28 */
    unsigned int      refreshes[8];  /* 0x2C */
} video_resolution_s; /* 76 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern video_resolution_s video_resolutions[];
extern int video_resolution_count;

#ifdef __cplusplus
}
#endif
