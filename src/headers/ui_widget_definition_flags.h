/* Bound to DB enum types_enum_values _2A6EDA2781F91EF7130057E6B1F8497E (2026-07-20 BLOCKED-retry). */
#ifndef UI_WIDGET_DEFINITION_FLAGS_H
#define UI_WIDGET_DEFINITION_FLAGS_H

/* ui_widget_definition_flags — bit indices for ui_widget_definition.flags. Values are BIT INDICES —
 * use as (1u << _bit).
 *
 * DB-AUTHORITATIVE (was a GUESS with only 2 bits before extraction). The IDA DB now carries the full
 * compiled "widget flags" sub-block ($2A6EDA27..., NUMBER_OF_WIDGET_FLAGS = 16). Canonical DB names
 * below; the 2 pre-existing consumer identifiers (widget_instance_initialize) are kept as same-value
 * aliases. This confirms the prior two guesses: pause_game_time = bit 1, render_regardless = bit 9. */
enum ui_widget_definition_flags {
/* Shared with widget_event_flags.h (same DB enum blob $2A6EDA27...). Guarded so a TU that includes
 * both headers does not redefine these enumerators — whichever header is seen first wins. The two
 * legacy consumer aliases below live OUTSIDE the guard (they exist only in this header). */
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
	_ui_widget_definition_pause_game_time_bit = 1,  /* legacy consumer alias */
	_ui_widget_definition_render_regardless_of_controller_index_bit = 9,  /* legacy consumer alias */
};

#endif /* UI_WIDGET_DEFINITION_FLAGS_H */
