/* game_time_statistics_new @0x8369A9B8 */
/* DEVIATION: game_time_statistics_paused/game_time_statistics_recording are unnamed adjacent globals; likely statistics.paused / .recording */
extern unsigned char game_time_statistics_paused;
extern unsigned char game_time_statistics_recording;

void game_time_statistics_new(void)
{
    game_time_statistics_recording = 1;
    game_time_statistics_paused = 0;
}
