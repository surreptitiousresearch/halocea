/* game_time_update @ 0x8369ABD0 — advance simulation by the number of ticks the elapsed
 * time warrants: run game_tick that many times (advancing local/server time), update the
 * director/observer cameras, and run one game_frame. When the clock is inactive only the
 * cameras update. determine_number_of_ticks is always called with is_peek=0 here.
 * hcex_pix_* are PIX profiling markers. */
#include <stdint.h>
#include "headers/game_time_globals.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern int determine_number_of_ticks(float time_delta_sec, uint8_t is_peek);
extern int16_t game_connection(void);
extern network_game_server *global_network_game_server_get(void);
extern void update_client_local_ticks(int16_t ticks_elapsed);
extern void director_update(float dt);
extern int director_is_first_person_player(void);
extern void observer_update(float dt, uint8_t predict_local_player_movement);
extern void game_tick(int ticks_remaining_this_frame);
extern void game_frame(float dt);
extern void hcex_pix_begin_event(const char *fmt, ...); /* DEVIATION: variadic (std r4..r10 save-area prologue @0x823CED10); was declared fixed-arity */
extern void hcex_pix_end_event(void);                   /* DEVIATION: void, not int (def: src/hcex/hcex_pix_end_event.cpp) */
extern void hcex_obj_sync_matrices_start(void);
extern int  hcex_cam_sync_remastered_mode(void);

void game_time_update(float time_delta_sec)
{
    int ticks;
    char clock_inactive = 0;

    /* is_peek is a constant 0 here (disasm: li r4,0); the caller-facing peek is phantom */
    ticks = determine_number_of_ticks(time_delta_sec, 0);
    if ( timeDemo )
        ticks = 1;

    switch ( game_connection() )
    {
        case _game_connection_network_server:
            global_network_game_server_get();
            break;
        case _game_connection_local:
            clock_inactive = game_time_globals->active == 0;
            if ( game_time_globals->active
              && (hcex_coop_local_player_index == -1 || hcex_coop_online_test_local) )
                update_client_local_ticks(ticks);
            break;
        default:
            break;
    }
    hcex_pix_end_event();

    if ( clock_inactive )
    {
        game_time_globals->last_local_time_elapsed = 0;
        if ( !halo_is_skip_private )
        {
            director_update(time_delta_sec);
            observer_update(time_delta_sec, director_is_first_person_player());
        }
        hcex_pix_begin_event("hcex_main_loop_end");
        return;
    }

    if ( hcex_coop_nticks_to_apply > 0 )
        hcex_coop_nticks_to_apply = 0;

    if ( ticks > 0 )
    {
        int tick = ticks - 1;
        int remaining = ticks;
        do
        {
            hcex_pix_begin_event("game_tick");
            game_tick(tick);
            hcex_pix_end_event();
            --remaining;
            --tick;
            game_time_globals->local_time = game_time_globals->local_time + 1;
            ++game_time_globals->server_time;
        }
        while ( remaining );
    }
    game_time_globals->last_local_time_elapsed = ticks;

    if ( !halo_is_skip_private )
    {
        hcex_obj_sync_matrices_start();
        hcex_pix_begin_event("observer_update");
        director_update(time_delta_sec);
        observer_update(time_delta_sec, director_is_first_person_player());
        hcex_pix_end_event();
        hcex_pix_begin_event("cam_sync");
        hcex_cam_sync_remastered_mode();
        hcex_pix_end_event();
    }

    hcex_pix_begin_event("game_frame");
    if ( game_connection() == _game_connection_network_client || game_connection() == _game_connection_network_server )
        game_frame((1.0f * time_delta_sec));
    else
        game_frame((game_time_globals->speed * time_delta_sec));
    hcex_pix_end_event();

    hcex_pix_begin_event("hcex_main_loop_end");
}
