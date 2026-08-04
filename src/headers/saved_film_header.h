#pragma once
/* saved_film_header — the decoded header packet at the start of a saved film (12 bytes, DB layout).
 * game_code is the 'blam' tag (0x626C616D); network_game_data_encoded_size is the size of the game-data
 * packet that follows. */

typedef struct saved_film_header
{
    unsigned int game_code;                      /* 0x0 */
    __int16      version;                         /* 0x4 */
    __int16      platform;                        /* 0x6 */
    __int16      network_game_data_encoded_size;  /* 0x8 */
    __int16      pad;                             /* 0xA */
} saved_film_header;                             /* 12 bytes */
