extern int delete_enumerated_saved_game_file(int index);

void player_profile_delete(int player_profile_index)
{
    if ( player_profile_index != -1 )
        delete_enumerated_saved_game_file(player_profile_index);
}
