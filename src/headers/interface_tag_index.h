#ifndef INTERFACE_TAG_INDEX_H
#define INTERFACE_TAG_INDEX_H

/* interface_tag_index — slot order of the interface (ui) tags fetched via interface_get_tag_index().
 * DB-verified: types_enum_values _39F31B983BBF7C85B0B0AA2004D4F55A carries these values verbatim. */
enum interface_tag_index {
	_interface_font_system = 0,
	_interface_font_terminal = 1,
	_interface_color_table_screen = 2,
	_interface_color_table_hud = 3,
	_interface_color_table_editor = 4,
	_interface_color_table_dialog = 5,
	_interface_hud_globals = 6,
	_interface_bitmap_motion_sweep = 7,
	_interface_bitmap_motion_sweep_mask = 8,
	_interface_bitmap_multiplayer_hud = 9,
	_interface_string_list_localization = 10,
	_interface_hud_digits = 11,
	_interface_bitmap_motion_blip = 12,
	_interface_bitmap_iface_map1 = 13,
	_interface_bitmap_iface_map2 = 14,
	_interface_bitmap_iface_map3 = 15,
	NUMBER_OF_INTERFACE_TAGS = 16,
};

#endif /* INTERFACE_TAG_INDEX_H */
