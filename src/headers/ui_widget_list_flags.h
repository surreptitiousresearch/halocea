/* Bound to DB enum types_enum_values _2A6EDA2781F91EF7130057E6B1F8497E (the "list flags" sub-block). */
#ifndef UI_WIDGET_LIST_FLAGS_H
#define UI_WIDGET_LIST_FLAGS_H

/* ui_widget_list_flags — bit indices for ui_widget_definition.list_flags (offset 336). Values are BIT
 * INDICES — use as (1u << _bit). DB-authoritative: same $HASH anon enum that carries the widget/event/
 * text-box sub-blocks; NUMBER_OF_LIST_FLAGS = 4. */
enum ui_widget_list_flags {
	_list_items_generated_in_code = 0,
	_list_items_generated_from_string_list_tag = 1,
	_list_items_only_one_tooltip_entry = 2,
	_list_single_preview_box_no_scroll = 3,
	NUMBER_OF_LIST_FLAGS = 4,
};

#endif /* UI_WIDGET_LIST_FLAGS_H */
