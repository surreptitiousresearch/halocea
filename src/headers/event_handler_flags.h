#ifndef EVENT_HANDLER_FLAGS_H
#define EVENT_HANDLER_FLAGS_H

/* event_handler_flags — bit indices for the event_handler flags word.
 * DB-verified: types_enum_values _09CA81194AB5D5A24E86BEA661A5D798 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum event_handler_flags {
	_event_handler_close_current_widget_bit = 0,
	_event_handler_close_other_widget_bit = 1,
	_event_handler_close_all_widgets_bit = 2,
	_event_handler_open_widget_bit = 3,
	_event_handler_reload_self_bit = 4,
	_event_handler_reload_widget_bit = 5,
	_event_handler_give_focus_to_widget = 6,
	_event_handler_run_function_bit = 7,
	_event_handler_replace_with_other_widget_bit = 8,
	_event_handler_go_back_to_previous_widget_bit = 9,
	_event_handler_run_scenario_script_bit = 10,
	_event_handler_look_for_conditional_widget_on_failure_bit = 11,
	NUMBER_OF_EVENT_HANDLER_FLAGS = 12,
	MAXIMUM_NUMBER_OF_EVENT_HANDLER_REFERENCES = 32,
};

#endif /* EVENT_HANDLER_FLAGS_H */

// event_handler_flags: DB-verified via types_enum_values _09CA81194AB5D5A24E86BEA661A5D798
