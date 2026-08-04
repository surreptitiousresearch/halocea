/* main_loop_init2 @ 0x8368B938 — second-pass init: map list, banlist, mapcycle, sound */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/blam_data_globals.h"

extern void main_setup_connection(void);
extern int64_t system_clocks(void);
extern uint32_t system_clocks_to_milliseconds(int64_t clocks);
extern void map_list_initialize_known_list(void);
extern void banlist_init(void);
extern void mapcycle_init(void);
extern uint8_t is_no_sounds_fmod(void);
extern void sound_disable_from_commandline(uint8_t disable);


void main_loop_init2(void)
{
    int64_t clocks;
    unsigned char no_sound;

    main_setup_connection();
    clocks = system_clocks();
    main_globals.last_time_clocks = clocks;
    main_globals.last_render_clocks = clocks;
    main_globals.last_time_msec = system_clocks_to_milliseconds(clocks);
    map_list_initialize_known_list();
    banlist_init();
    mapcycle_init();

    no_sound = 0;
    if ( optionNoSound || is_no_sounds_fmod() )
        no_sound = 1;
    sound_disable_from_commandline(no_sound);

    game_initial_loading = 0;
}
