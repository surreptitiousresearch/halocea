#pragma once
/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _9EEFAB91EEC73AFBE98723188AB35096 exactly (DB uses the _widget_type_* prefix; the _ui_widget_type_*
 * identifiers below are the consumer-facing names and are kept). */

/* GUESS: enum recovered by reconciliation, no ground-truth source — rationale: widget_instance.widget_type
 * dispatch in widget_instance_render_recursive @0x83734E48 calls widget_instance_render_text_box for 1,
 * widget_instance_render_spinner_list for 2, widget_instance_render_column_list for 3 (0 renders as a plain
 * container, only recursing into children). Ordering corroborated by the ui_widget_definition tag layout
 * (headers_ref), whose per-type parameter blocks appear in the order text_box(0x134), spinner_list(0x184),
 * column_list(0x1B4), game_model(0x1D4), movie(0x2E0), custom(0x360). */
enum ui_widget_type {
	_ui_widget_type_container = 0,
	_ui_widget_type_text_box = 1,
	_ui_widget_type_spinner_list = 2,
	_ui_widget_type_column_list = 3,
	_ui_widget_type_game_model = 4,
	_ui_widget_type_movie = 5,
	_ui_widget_type_custom = 6,
	NUMBER_OF_UI_WIDGET_TYPES = 7,
};
