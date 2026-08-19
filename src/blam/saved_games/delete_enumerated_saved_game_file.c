/* delete_enumerated_saved_game_file @0x83726B20 — in this release, forgets the cached "last used player 1
 * profile" index for the file and reports failure (returns 0). */

extern void reset_last_player1_profile_index(int index);

int delete_enumerated_saved_game_file(int index)
{
    reset_last_player1_profile_index(index);
    return 0;
}
