/* widget_globals @ 0x8445FEB0 (.data, 68 bytes)
 * DB applied_types: _widget_globals widget_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 active_widgets             = 0x0000000000000000
 *   +0x08 widget_stack               = 0x0000000000000000
 *   +0x10 current_system_milliseconds = 0x00000000
 *   +0x14 pause_disabled_ticks       = 0x00000000
 *   +0x18 main_menu_deferred_error_code = 0x0000
 *   +0x1A pause_game_time_count      = 0x0000
 *   +0x1C fade_to_black              = 0x00000000 -> 0f
 *   +0x20 deferred_error             = 0x000000000000000000000000
 *   +0x2C deferred_dashboard_error   = 0x00000000
 *   +0x30 deferred_cinematic_error   = 0x0000000000000000
 *   +0x38 initialization_thread      = 0x00000000
 *   +0x3C filesystem_check_result    = 0x0000
 *   +0x3E initialized                = 0x00
 *   +0x3F dont_load_children_recursive = 0x00
 *   +0x40 debug_show_path            = 0x00
 *   +0x41 processing_inhibited       = 0x00
 *   +0x42 main_menu_music_active     = 0x00
 *   +0x43 sound_paused               = 0x00
 * data-bss (68 bytes).
 */
#include "../headers/widget_globals.h"

_widget_globals widget_globals;
