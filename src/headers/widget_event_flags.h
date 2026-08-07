#ifndef WIDGET_EVENT_FLAGS_H
#define WIDGET_EVENT_FLAGS_H

/* widget_event_flags — widget_event flags.
 * DB-verified: types_enum_values _2A6EDA2781F91EF7130057E6B1F8497E carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit).
 *
 * The list sub-block of the same $2A6EDA27 blob has a canonical home of its own; it was defined
 * a second time here until 2026-08-07 (an ODR redefinition of five enumerators). Included rather
 * than repeated, so the enumerators have exactly one body. */
#include "ui_widget_list_flags.h"
enum widget_event_flags {
	_widget_event_button_a = 0,
	_widget_event_button_b = 1,
	_widget_event_button_x = 2,
	_widget_event_button_y = 3,
	_widget_event_button_black = 4,
	_widget_event_button_white = 5,
	_widget_event_button_left_trigger = 6,
	_widget_event_button_right_trigger = 7,
	_widget_event_button_dpad_up = 8,
	_widget_event_button_dpad_down = 9,
	_widget_event_button_dpad_left = 10,
	_widget_event_button_dpad_right = 11,
	_widget_event_button_start = 12,
	_widget_event_button_back = 13,
	_widget_event_button_left_thumb = 14,
	_widget_event_button_right_thumb = 15,
	NUMBER_OF_WIDGET_EVENT_BUTTONS = 16,
	_widget_event_left_analog_stick_up = 16,
	_widget_event_left_analog_stick_down = 17,
	_widget_event_left_analog_stick_left = 18,
	_widget_event_left_analog_stick_right = 19,
	_widget_event_right_analog_stick_up = 20,
	_widget_event_right_analog_stick_down = 21,
	_widget_event_right_analog_stick_left = 22,
	_widget_event_right_analog_stick_right = 23,
	_widget_event_created = 24,
	_widget_event_deleted = 25,
	_widget_event_get_focus = 26,
	_widget_event_lose_focus = 27,
	_widget_event_mouse_left = 28,
	_widget_event_mouse_middle = 29,
	_widget_event_mouse_right = 30,
	_widget_event_mouse_double_click = 31,
	_widget_event_custom_activation = 32,
	_widget_event_post_render = 33,
	NUMBER_OF_WIDGET_EVENTS = 34,
/* Shared with ui_widget_definition_flags.h (same DB enum blob $2A6EDA27...). Guarded so a TU that
 * includes both headers does not redefine these enumerators — whichever header is seen first wins. */
#ifndef WIDGET_DEFINITION_FLAG_BITS
#define WIDGET_DEFINITION_FLAG_BITS
	_widget_pass_unhandled_events_to_children_bit = 0,
	_widget_pause_game_time_bit = 1,
	_widget_flash_background_bitmap_bit = 2,
	_widget_dpad_updown_tabs_thru_children_bit = 3,
	_widget_dpad_leftright_tabs_thru_children_bit = 4,
	_widget_dpad_updown_tabs_thru_list_items_bit = 5,
	_widget_dpad_leftright_tabs_thru_list_items_bit = 6,
	_widget_dont_focus_a_specific_child_bit = 7,
	_widget_pass_unhandled_events_to_all_children_bit = 8,
	_widget_render_regardless_of_controller_index_bit = 9,
	_widget_pass_handled_events_to_all_children_bit = 10,
	_widget_return_to_main_menu_if_no_history_bit = 11,
	_widget_always_use_tag_controller_index_bit = 12,
	_widget_always_render_with_nifty_fx_bit = 13,
	_widget_dont_push_history_data_bit = 14,
	_widget_force_handle_mouse_data_bit = 15,
	NUMBER_OF_WIDGET_FLAGS = 16,
#endif
	_text_box_editable_bit = 0,
	_text_box_password_bit = 1,
	_text_box_flashing_text_bit = 2,
	_text_box_no_weird_focus_test = 3,
	NUMBER_OF_TEXT_BOX_FLAGS = 4,
	/* the list sub-block lives in ui_widget_list_flags.h, included above */
	_widget_controller0 = 0,
	_widget_controller1 = 1,
	_widget_controller2 = 2,
	_widget_controller3 = 3,
	_widget_controller_any = 4,
	NUMBER_OF_WIDGET_CONTROLLER_OPTIONS = 5,
};

#endif /* WIDGET_EVENT_FLAGS_H */

// widget_event_flags: DB-verified via types_enum_values _2A6EDA2781F91EF7130057E6B1F8497E
