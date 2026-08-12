/* game_end_teaser_start @0x83784FD8 — DEVIATION: the decompiler rendered `sound_stop_all(v3)` as if it
 * took ui_stop_main_menu_music's return value as an argument, but both callees are true void-void
 * functions (ui_stop_main_menu_music.c, sound_stop_all.c) and disasm_range(0x83784FD8,0x8378501C) shows
 * two independent `bl` calls with no register moved between them — the decompiler mis-paired them. */

#include <stdint.h>
#include "headers/bink_playback_flags.h"

extern void ui_stop_main_menu_music(void);
extern void sound_stop_all(void);
extern char *attract_mode_get_localized_movie_path(int16_t movie);
extern void bink_playback_start(const char *full_pathname, unsigned int flags);

void game_end_teaser_start(uint8_t looping)
{
    unsigned int flags = (1u << _bink_playback_prevent_events_to_ui_bit)
                       | (1u << _bink_playback_dont_render_ui_bit)
                       | (1u << _bink_playback_return_to_main_menu_when_finished_bit);

    ui_stop_main_menu_music();
    sound_stop_all();
    if ( looping )
        flags |= (1u << _bink_playback_loop_bit)
               | (1u << _bink_playback_button_click_stops_movie_bit);

    bink_playback_start(attract_mode_get_localized_movie_path(5), flags);
}
