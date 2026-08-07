/* network_game_get_number_of_games_played @0x83756A98 — reads network_game_data.number_of_games_played
 * (offset 0x548, DB types_members). It IS a struct field; only the base is gone, folded to NULL by the
 * compiler because network_game_server_get_game() is a `li r3, 0; blr` stub. The body is one
 * `lwz r3, 0x548` = 80600548, rA = r0. Mechanism and evidence: network_game_server_get_game.c. */

int network_game_get_number_of_games_played(void)
{
    return *(int *)0x548;
}
