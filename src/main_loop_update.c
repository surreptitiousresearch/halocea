/* main_loop_update @ 0x8368B9F0 — per-frame standalone Blam update.
 * Returns 1 to keep looping, 0 to exit. Handles map state transitions,
 * input, networking per-frame, game time, rendering and sound. */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/data_iterator.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/error_type.h"
#include "headers/single_player_progress_flags.h"
#include "headers/blam_data_globals.h"

extern uint8_t game_in_editor(void);
extern void main_reset_map_private(void);
extern uint8_t scenario_switch_structure_bsp(int16_t structure_bsp_index);
extern void hud_load(uint8_t begin);
extern uint8_t game_time_get_paused(void);
extern int16_t local_player_count(void);
extern uint8_t cinematic_in_progress(void);
extern int game_time_get(void);
extern void hcex_output_dbg(const char *, ...);
extern uint8_t players_respawn_coop(void);
extern void game_state_save(uint8_t do_autosave);
extern void hud_autosave(uint8_t begin);
extern void main_change_map_name(void);
extern void game_state_revert(void);
extern void ui_widgets_disable_pause_game(int duration_ticks);
extern uint8_t cinematic_can_be_skipped(void);
extern void hcex_fire_event(const char *);
extern void main_reset_map_private(void);
extern void game_state_save_core(const char *name);
extern void game_state_load_core(const char *name);
extern void main_menu_load(void);
extern void xbox_demos_launch(void);
extern void main_skip_private(void);
extern uint8_t cache_files_precache_in_progress(void);
extern int16_t cache_files_precache_map_status(float *progress);
extern void cache_files_precache_map_end(void);
extern uint8_t bink_playback_active(void);
extern uint8_t cache_files_precache_map_begin(const char *name, uint8_t blocking);
extern void ui_loading_init_state(void);
extern wchar_t *ascii_to_wide(const char *ascii, wchar_t *unicode, unsigned int unicode_length_bytes);
extern void display_error_when_main_menu_loaded(int16_t error_code);
extern void input_frame_begin(void);
extern void input_update(void);
extern void input_abstraction_update(void);
extern void shell_idle(void);
extern void event_manager_update(void);
extern uint8_t main_menu_screen_is_active(void);
extern void net_graph_update(void);
extern uint8_t shell_application_is_paused(void);
extern uint8_t network_game_client_start_frame(void);
extern network_game_client *global_network_game_client_get(void);
extern void network_game_abort(void);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_start_frame(void);
extern uint8_t save_film_playback_next_game_tick(void);
extern void hcex_coop_try_to_apply_player_actions(void);
extern void main_update_time(void);
extern void process_ui_widgets(void);
extern void bink_playback_update(void);
extern uint8_t input_abstraction_is_idle(void);
extern uint8_t console_is_active(void);
extern uint8_t game_in_progress(void);
extern uint8_t main_menu_is_active(void);
extern int terminal_update(void);
extern uint8_t console_update(void);
extern void main_save_map_private(void);
extern void debug_keys_update(void);
extern void cheats_update(void);
extern int determine_number_of_ticks(float time_delta_sec, uint8_t is_peek);
extern int player_control_update(float seconds_elapsed, int ticks_to_apply_this_update_to);
extern void game_time_update(float time_delta_sec);
extern void game_engine_update_non_deterministic(float seconds_elapsed);
extern int hud_chat_update(void);
extern uint8_t network_game_client_end_frame(int ticks_to_apply_this_frames_update_to, uint8_t did_time_overflow_occur);
extern int64_t system_clocks(void);
extern float game_time_get_since_tick(void);
extern double system_clock_delta_seconds_real(int64_t a, int64_t b);
extern void hcex_pix_begin_event(const char *fmt, ...); /* DEVIATION: variadic (std r4..r10 save-area prologue @0x823CED10); the two trailing doubles below are varargs, not fixed params */
extern void sound_render(void);
extern void hcex_pix_end_event(void);
extern void main_pregame_render(void);
extern uint8_t editor_should_exit(void);
extern uint8_t game_engine_running(void);
extern void xbox_dashboard_launch(void);
extern void input_frame_end(void);
extern uint32_t system_clocks_to_milliseconds(int64_t clocks);
extern int16_t main_get_solo_level_from_name(const char *name);
extern void player_profile_save_level_completed(int16_t local_player_index);
extern void ui_set_next_level(int16_t level);
extern uint32_t system_milliseconds(void);


extern void main_fade_to_dashboard(void);
int main_loop_update(void)
{
    data_iterator iter;
    int connection;

    if ( game_in_editor() )
    {
        if ( main_globals.reset_map )
            main_reset_map_private();
        goto frame;
    }

    if ( (unsigned short)main_globals.switch_to_structure_bsp_index != 0xFFFF )
    {
        scenario_switch_structure_bsp(main_globals.switch_to_structure_bsp_index);
        main_globals.switch_to_structure_bsp_index = -1;
        hud_load(0);
    }

    if ( main_globals.lost_map )
    {
        if ( !game_time_get_paused() )
        {
            if ( main_globals.loss_timer++ > 90 )
            {
                main_globals.lost_map = 0;
                main_globals.loss_timer = 0;
                if ( hcex_reset_single_on_die && local_player_count() == 1 )
                    main_globals.reset_map = 1;
                else
                    main_globals.revert_map = 1;
                main_globals.switch_to_structure_bsp_index = -1;
                main_globals.save_map = 0;
                main_globals.lost_map = 0;
            }
        }
        if ( main_globals.revert_map )
            return 1;
    }

    if ( main_globals.won_map )
    {
        /* level complete: record completion for every spawned player, queue next level */
        short next_level;
        short i;

        main_globals.want_to_be_at_main_menu = 1;
        main_globals.won_map = 0;
        next_level = (short)(main_get_solo_level_from_name(main_globals.soloplayer_map_name) + 1);
        if ( next_level >= NUMBER_OF_SINGLE_PLAYER_LEVELS )
            next_level = -1;
        if ( player_spawn_count > 0 )
        {
            i = 0;
            do
                player_profile_save_level_completed(i++);
            while ( i < player_spawn_count );
        }
        ui_set_next_level(next_level);
        return 1;
    }

    if ( main_globals.respawn )
    {
        if ( !game_time_get_paused() && !cinematic_in_progress() )
        {
            if ( main_globals.respawn_timer++ > 90 )
            {
                hcex_output_dbg("main_respawn_private tick %d\n", game_time_get());
                if ( players_respawn_coop() )
                {
                    main_globals.respawn = 0;
                    main_globals.respawn_timer = 0;
                }
            }
        }
    }

    if ( main_globals.saving_map )
    {
        main_globals.saving_map = 0;
        if ( !main_globals.revert_map )
        {
            hcex_output_dbg("main_saving_map_private tick %d\n", game_time_get());
            game_state_save(1);
            hud_autosave(0);
        }
    }

    if ( main_globals.rename_map )
        main_change_map_name();

    if ( main_globals.revert_map )
    {
        hcex_output_dbg("main_revert_map_private tick %d\n", game_time_get());
        game_state_revert();
        ui_widgets_disable_pause_game(30);
        main_globals.revert_map = 0;
    }

    if ( main_globals.skip_cinematic )
    {
        unsigned char want_revert;

        if ( cinematic_can_be_skipped() )
        {
            hcex_fire_event("cinameticStop");
            main_globals.revert_map = 1;
            main_globals.switch_to_structure_bsp_index = -1;
            want_revert = 1;
            main_globals.save_map = 0;
            main_globals.lost_map = 0;
        }
        else
        {
            want_revert = main_globals.revert_map;
        }
        main_globals.skip_cinematic = 0;
        if ( want_revert )
            return 1;
    }

    if ( main_globals.reset_map )
        main_reset_map_private();

    if ( main_globals.save_core )
    {
        game_state_save_core("core.bin");
        main_globals.save_core = 0;
    }

    if ( main_globals.load_core )
    {
        game_state_load_core("core.bin");
        main_globals.load_core = 0;
    }

    if ( main_globals.want_to_be_at_main_menu )
        main_menu_load();

    if ( main_globals.run_xdemos )
    {
        main_globals.run_xdemos = 0;
        xbox_demos_launch();
    }

    if ( main_globals.cutscene_skip )
        main_skip_private();

    if ( main_globals.queue_map )
    {
        float status_progress;

        if ( cache_files_precache_in_progress()
          && cache_files_precache_map_status(&status_progress) == 1 )
            cache_files_precache_map_end();

        if ( !cache_files_precache_in_progress() && !bink_playback_active() )
        {
            cache_files_precache_map_begin(main_globals.queued_map_name, 0);
            main_globals.queue_map = 0;
        }
    }

    if ( main_globals.want_to_connect )
    {
        unsigned short wide_password[18];

        ui_loading_init_state();
        ascii_to_wide(main_globals.connect_password, wide_password, 0x12);
        display_error_when_main_menu_loaded(_error_invalid_address);
        main_globals.want_to_be_at_main_menu = 1;
        main_globals.switch_to_structure_bsp_index = -1;
        main_globals.save_map = 0;
        main_globals.connect_address[0] = 0;
        main_globals.connect_password[0] = 0;
        main_globals.want_to_connect = 0;
    }

frame:
    connection = main_globals.connection;

    {
        /* attested void chain: decompiler threaded r3 through these per-frame updates */
        input_frame_begin();
        input_update();
        input_abstraction_update();
        shell_idle();

        if ( main_globals.want_to_exit )
            return 0;

        event_manager_update();
        if ( connection == _game_connection_network_client || connection == _game_connection_network_server || main_menu_screen_is_active() )
            net_graph_update();
    }

    if ( shell_application_is_paused() && connection != _game_connection_network_client && connection != _game_connection_network_server )
        goto end_of_frame;

    {
        /* per-connection-type start of frame */
        switch ( connection )
        {
            case _game_connection_network_client:
                if ( !network_game_client_start_frame() )
                {
                    global_network_game_client_get();
                    display_error_when_main_menu_loaded(_error_network_connection_lost);
                    network_game_abort();
                }
                break;
            case _game_connection_network_server:
            {
                unsigned char ok;
                global_network_game_server_get();
                ok = network_game_client_start_frame();
                if ( ok == 1 )
                    ok = network_game_server_start_frame();
                if ( ok != 1 )
                {
                    display_error_when_main_menu_loaded(_error_network_generic);
                    network_game_abort();
                }
                break;
            }
            case _game_connection_film_playback:
                if ( !save_film_playback_next_game_tick() )
                    return 0;
                break;
        }
    }

    hcex_coop_try_to_apply_player_actions();
    main_update_time();
    process_ui_widgets();
    bink_playback_update(); /* attested void(void) */

    if ( input_abstraction_is_idle() && !console_is_active() )
    {
        if ( bink_playback_active()
          || (game_in_progress() && !game_time_get_paused() && cinematic_in_progress()) )
        {
            main_globals.idle_last_interesting = system_milliseconds();
        }
        else if ( main_globals.idle_timeout > 0 )
        {
            int activity_left = main_globals.idle_timeout + main_globals.idle_last_activity
                              - system_milliseconds();
            int interesting_left = main_globals.idle_last_interesting
                                 - system_milliseconds() + 10000;
            if ( activity_left <= 0 && interesting_left <= 0 )
            {
                if ( main_menu_is_active() )
                {
                    /* DEVIATION: collapsed inlined copy of main_fade_to_dashboard@0x83688ED0 (zero-xref donor, void(void), no args to fold; host wrote the same 3 main_globals fields in a different but equivalent order) into a direct call. */
                    main_fade_to_dashboard();
                }
                else
                {
                    main_globals.want_to_be_at_main_menu = 1;
                    main_globals.switch_to_structure_bsp_index = -1;
                    main_globals.save_map = 0;
                }
            }
        }
    }
    main_globals.idle_last_activity = system_milliseconds();

    if ( game_in_progress() )
    {
        int ticks;
        unsigned char cheats;
        float delta_seconds;
        unsigned char did_simulate;

        terminal_update();
        if ( console_update() && !main_globals.connection )
            goto post_sim;   /* console paused: skip simulation, go straight to save/render */

        debug_keys_update();
        cheats_update();
        cheats = 0;   /* cheats_update is a no-op stub; r3 stays the li 0 arg */
        delta_seconds = ((float)main_globals.halt_time_scale * main_globals.seconds_elapsed);
        ticks = determine_number_of_ticks(delta_seconds, cheats);
        player_control_update(delta_seconds, ticks);

        if ( connection == _game_connection_network_server )
            global_network_game_server_get();

        if ( connection != _game_connection_network_client && connection != _game_connection_network_server )
            goto advance_time;

        hud_chat_update();
        if ( !network_game_client_end_frame(ticks, main_globals.did_time_overflow_occur) )
        {
            display_error_when_main_menu_loaded(_error_network_generic);
            network_game_abort();
        }

    advance_time:
        game_time_update(delta_seconds);
        if ( connection == _game_connection_network_client && client_log_destination )
        {
            player_datum *player;
            data_iterator_new(&iter, player_data);
            player = data_iterator_next(&iter);
            while ( player )
            {
                /* local_player_index is signed __int16; keep the unsigned cast for the 0xFFFF sentinel compare */
                if ( (uint16_t)player->local_player_index != 0xFFFF )
                {
                    if ( player->unit_index != -1 )
                        global_network_game_client_get();
                    break;
                }
                player = data_iterator_next(&iter);
            }
        }

        did_simulate = shell_application_is_paused() == 0;
        game_engine_update_non_deterministic(
            ((float)main_globals.halt_time_scale * main_globals.seconds_elapsed));

    post_sim:
        if ( main_globals.save_map )
            main_save_map_private();

        did_simulate = 1;
        if ( timeDemo )
        {
            did_simulate = 0;
            if ( game_time_get() != last_time )
            {
                did_simulate = 1;
                last_time = game_time_get();
            }
        }

        if ( did_simulate )
        {
            if ( !debug_no_drawing )
            {
                int64_t render_clocks = system_clocks();
                float since_render = 0.0f;
                if ( !game_time_get_paused() && !console_is_active() )
                {
                    game_time_get_since_tick();
                    since_render = system_clock_delta_seconds_real(main_globals.last_render_clocks,
                                                                   render_clocks);
                }
                hcex_do_legacy_render = 1;
                hcex_pix_begin_event("sound_render", since_render, 0.0);
                sound_render(); /* attested void: r3-thread into the PIX end-marker's phantom arg dropped */
                hcex_pix_end_event();
                main_globals.last_render_clocks = render_clocks;
            }
            else
            {
                sound_render();
            }
        }
        goto end_of_frame;
    }

    if ( !timeDemo && !shell_application_is_paused() )
        main_pregame_render();

end_of_frame:
    if ( main_globals.want_to_exit )
        return 0;

    if ( editor_should_exit() && !game_engine_running() )
    {
        main_globals.reset_map = 1;
        main_globals.switch_to_structure_bsp_index = -1;
        main_globals.save_map = 0;
        main_globals.lost_map = 0;
    }

    {
        int dashboard = 0;
        if ( main_globals.exit_to_dashboard )
        {
            dashboard = bink_playback_active();
            if ( !dashboard )
                xbox_dashboard_launch();
        }
        input_frame_end(); /* attested void(void): r3-thread arg removed */
    }

    if ( main_globals.restart_time )
    {
        int64_t clocks = system_clocks();
        main_globals.restart_time = 0;
        main_globals.last_time_clocks = clocks;
        main_globals.last_render_clocks = clocks;
        main_globals.last_time_msec = system_clocks_to_milliseconds(clocks);
        main_globals.halt_time_scale = 1;
    }
    return 1;
}
