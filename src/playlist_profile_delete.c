/* playlist_profile_delete @0x83782070 */
extern int delete_enumerated_saved_game_file(int index);

void playlist_profile_delete(int playlist_profile_index)
{
    if ( playlist_profile_index != -1 )
        delete_enumerated_saved_game_file(playlist_profile_index);
}
