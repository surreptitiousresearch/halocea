/* main_setup_connection @ 0x8368B538 — decide initial connection state at boot:
 * jump to the main menu, start film playback of the last recording, or load the
 * configured solo level directly. */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/game_globals.h"
#include "headers/network_game_data.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *, int, unsigned int);
extern char *strrchr(const char *, int);
extern int sprintf_0(char *string, const char *format, ...);
extern uint16_t save_film_get_mode(void);
extern uint8_t save_film_open_recording(const char *filename);
extern uint8_t save_film_begin_playback(network_game_data *game);
extern void game_options_new(game_options *options);
extern void game_precache_new_map(const char *map_name, uint8_t blocking);
extern void game_dispose_from_old_map(void);
extern void main_new_map(game_options *options);
extern void main_menu_load(void);
extern char *strncpy(char *, const char *, unsigned int);


void main_setup_connection(void)
{
    game_options options;
    network_game_data film_data;

    /* clear want_to_be_at_main_menu unless we are replaying the last recording */
    if ( main_globals.playback_last_recording )
        main_globals.want_to_be_at_main_menu = 0;

    if ( main_globals.want_to_be_at_main_menu )
    {
        main_menu_load();
        return;
    }

    if ( main_globals.playback_last_recording )
    {
        memset(&film_data, 0, sizeof(film_data));
        main_globals.connection = _game_connection_film_playback;
        if ( !save_film_get_mode() )
        {
            char *leaf = strrchr(main_globals.soloplayer_map_name, '\\');
            sprintf_0((char *)&options, "%s%s.%s", "d:\\", leaf + 1, "flm");
            if ( !save_film_open_recording((char *)&options)
              || !save_film_begin_playback(&film_data) )
            {
                main_globals.want_to_be_at_main_menu = 1;
                main_menu_load();
            }
        }
    }
    else
    {
        main_globals.connection = _game_connection_local;
        game_options_new(&options);
        strncpy(options.map_name, main_globals.soloplayer_map_name, 0xFF);
        options.map_name[255] = 0;
        options.difficulty = global_difficulty_level;
        game_precache_new_map(options.map_name, 1);
        game_dispose_from_old_map();
        main_new_map(&options);
    }
}
