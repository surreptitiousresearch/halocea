/* game_state_dispose @ 0x836845B0 — free the game-state buffer and close its file */
extern void game_state_free_buffer(void);
extern void game_state_close_file(void);

void game_state_dispose(void)
{
    /* Two independent sequential calls; close_file takes no argument (reads globals).
     * The prior `close_file(free_buffer())` was a decompiler r3-threading artifact:
     * disasm shows no r3 set between the two bl's and close_file has no incoming arg. */
    game_state_free_buffer();
    game_state_close_file();
}
