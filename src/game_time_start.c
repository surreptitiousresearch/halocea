/* game_time_start @0x8369AA00 — start the game clock at normal speed and start the appropriate network
 * time sync: client-side for client roles, server-side otherwise.
 *
 * DEVIATION: the decompiler's tangled role test reduces to "connection is a client role"
 * (connection != 0 and connection != 2); roles 1 and 3 take the client path, 0 and 2 the server path. */

#include "headers/game_time_globals.h"
#include <stdint.h>

extern unsigned char game_time_statistics_recording;
extern unsigned char game_time_statistics_paused;
extern int16_t game_connection(void);
extern void *update_client_start(void);
extern void update_server_start(void);

/* Attested void: r3 at both blr paths is callee residue (update_*_start), 0/6 callers consume. */
void game_time_start(void)
{
    game_time_statistics_recording = 1;
    game_time_statistics_paused = 0;
    game_time_globals->speed = 1.0f;
    game_time_globals->active = 1;
    game_time_globals->leftover_time_sec = 0.0f;

    int connection = game_connection();
    if ( (unsigned int)(short)connection <= 3 )
    {
        if ( connection != 0 && connection != 2 )
            update_client_start();
        else
            update_server_start();
    }
}
