#pragma once
/* _playlist_profile_globals @ playlist_profile_globals (playlist_profile.c). Layout per database. */

#include "playlist_profile_write_internal_input.h"

typedef struct thread thread;

typedef struct _playlist_profile_globals
{
    playlist_profile_write_internal_input thread_input; /* 0x00 */
    thread          *thread;                          /* 0x9C */
    unsigned short   number_of_default_profiles;      /* 0xA0 */
    unsigned char    initialized;                     /* 0xA2 */
} _playlist_profile_globals;

extern _playlist_profile_globals playlist_profile_globals;
