#pragma once
/* history_playback_statistics — running statistics of player_update_history_play replays, embedded in
 * player_update_history at +0x0C (DB types_members-confirmed, 24 bytes). */

typedef struct history_playback_statistics
{
    int   number_of_playbacks;                                       /* 0x00 */
    int   total_updates_played_back;                                 /* 0x04 */
    int   total_ticks_played_back;                                   /* 0x08 */
    float total_difference_between_client_prediction_and_playback;   /* 0x0C */
    float average_difference_between_client_prediction_and_playback; /* 0x10 */
    float average_ticks_played_back;                                 /* 0x14 */
} history_playback_statistics;                                       /* 24 bytes */
