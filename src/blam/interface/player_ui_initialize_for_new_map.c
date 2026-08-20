/* player_ui_initialize_for_new_map @0x83699390
 *
 * CAVEAT — faithfully reconstructed: the shipped function is a single `blr` @0x83699390 and does
 * nothing. It is nevertheless still called, 2nd in the per-map sweep in game_initialize_for_new_map
 * (right after hcex_obj_clear, before the RNG seed), and that sweep also ends on an empty function
 * (ui_widgets_safe_to_load(1)). The empty body is the binary, not missing work. */
void player_ui_initialize_for_new_map(void) {}
