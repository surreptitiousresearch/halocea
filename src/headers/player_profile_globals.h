#pragma once
/* _player_profile_globals @ player_profile_globals (player_profile.c). Layout per database. */

#include "player_profile.h"
#include "player_profile_write_internal_input.h"

typedef struct thread thread;

typedef struct _player_profile_globals
{
    player_profile default_profile;      /* 0x000 */
    player_profile_write_internal_input thread_input; /* 0x7BC */
    thread        *thread;               /* 0xF7C */
    unsigned char  initialized;          /* 0xF80 */
} _player_profile_globals;

#ifdef __cplusplus
extern "C" {
#endif

extern _player_profile_globals player_profile_globals;

#ifdef __cplusplus
}
#endif
