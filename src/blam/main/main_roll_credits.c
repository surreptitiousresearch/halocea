/* main_roll_credits @0x8368B478 — finish the game and roll the end-game credits: mark the game finished, load
 * the main menu, then load the credits directly for the end-game flow.
 *
 * Deviation: the chained r3 return values are ABI noise (player_ui_finished_game / main_menu_load /
 * credits_load_directly_for_endgame are independent calls); reproduced as a sequence. */

extern void player_ui_finished_game(void);
extern void main_menu_load(void);
typedef struct widget_instance widget_instance;
extern widget_instance *credits_load_directly_for_endgame(void);

void main_roll_credits(void)
{
    player_ui_finished_game();
    main_menu_load();
    credits_load_directly_for_endgame();
}
