#include <stdint.h>

extern void ui_stop_main_menu_music(void);
extern void sound_stop_all(void);
extern uint8_t ui_main_menu_music_active(void);
extern void ui_start_main_menu_music(void);

void game_end_credits_start(uint8_t in_beginscene)
{int stop_arg;
    ui_stop_main_menu_music();
    sound_stop_all();
    if ( !ui_main_menu_music_active() )
        ui_start_main_menu_music();
}
