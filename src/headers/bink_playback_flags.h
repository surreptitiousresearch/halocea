#ifndef BINK_PLAYBACK_FLAGS_H
#define BINK_PLAYBACK_FLAGS_H

/* bink_playback_flags — bink_playback flags.
 * DB-verified: types_enum_values _6FED4AFAB50D37A00AE8EC857DBE099A carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum bink_playback_flags {
	_bink_playback_loop_bit = 0,
	_bink_playback_button_click_stops_movie_bit = 1,
	_bink_playback_prevent_events_to_ui_bit = 2,
	_bink_playback_dont_render_ui_bit = 3,
	_bink_playback_full_screen_bit = 4,
	_bink_playback_return_to_main_menu_when_finished_bit = 5,
	_bink_playback_dont_allow_skipping_if_filesystem_check_thread_is_active_bit = 6,
	_bink_playback_eat_up_memory_like_a_goddamn_beaver_bit = 7,
	NUMBER_OF_BINK_PLAYBACK_FLAGS = 8,
};

#endif /* BINK_PLAYBACK_FLAGS_H */

// bink_playback_flags: DB-verified via types_enum_values _6FED4AFAB50D37A00AE8EC857DBE099A
