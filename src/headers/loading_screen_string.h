#pragma once

/* loading_screen_string — indices into the "ui\shell\strings\loading" unicode string list drawn by
 * ui_loading_update_and_render.
 * Source: reference enum _FCE9DF82E67109FDA5DC72C27C494BF8 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum loading_screen_string {
	loading_string_natneg = 0,
	loading_string_server = 1,
	loading_string_connecting = 2,
	loading_string_retrying = 3,
	loading_string_connected = 4,
	loading_string_connected_loading = 5,
	loading_string_server_loading = 6,
	loading_string_press_escape = 7,
	loading_string_waiting_for_new_game = 8,
	loading_string_esrb_warning = 9,
};

// loading_screen_string: DB-verified via types_enum_values _FCE9DF82E67109FDA5DC72C27C494BF8 (anonymous compiled enum, names sans leading _)
