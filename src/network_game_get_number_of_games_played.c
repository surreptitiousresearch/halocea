/* network_game_get_number_of_games_played @0x83756A98 — reads a persistent counter at the fixed low
 * memory address 0x548 (confirmed via disasm: `lwz r3, 0x548` with no base register, i.e. absolute
 * addressing, not a struct field). Address kept as a raw literal since no symbol/type is registered
 * for it in the database. */

int network_game_get_number_of_games_played(void)
{
    return *(int *)0x548;
}
