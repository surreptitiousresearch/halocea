/* Bound to DB enum types_enum_values _2A6EDA2781F91EF7130057E6B1F8497E (the "list flags" sub-block). */
#ifndef UI_WIDGET_LIST_FLAGS_H
#define UI_WIDGET_LIST_FLAGS_H

/* ui_widget_list_flags — bit indices for ui_widget_definition.list_flags (offset 336). Values are BIT
 * INDICES — use as (1u << _bit). DB-authoritative: same $HASH anon enum that carries the widget/event/
 * text-box sub-blocks; these five are its value_index 58..62. Per-enumerator provenance below —
 * headers_ref carries no body for this enum, so the DB is the sole evidence and it has all five. */
enum ui_widget_list_flags {
	_list_items_generated_in_code = 0,              /* types_enum_values $2A6EDA27 value_index 58 */
	_list_items_generated_from_string_list_tag = 1, /* types_enum_values $2A6EDA27 value_index 59 */
	_list_items_only_one_tooltip_entry = 2,         /* types_enum_values $2A6EDA27 value_index 60 */
	_list_single_preview_box_no_scroll = 3,         /* types_enum_values $2A6EDA27 value_index 61 */
	NUMBER_OF_LIST_FLAGS = 4,                       /* types_enum_values $2A6EDA27 value_index 62 */
};

#endif /* UI_WIDGET_LIST_FLAGS_H */
