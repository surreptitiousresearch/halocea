/* DB-verified 2026-07-20 (BLOCKED-retry): red=0 / blue=1 are now PROVEN by the compiled enum
 * types_enum_values _5D2EF8FA442E585DBFF707A5F144D58C, whose multiplayer sub-block is
 * _team_red=0, _team_blue=1, MAXIMUM_MULTIPLAYER_GAME_TEAMS=2. The prior GUESS on the color labels is
 * confirmed. The _multiplayer_team_* identifiers below are the consumer-facing names and are kept. */
#ifndef MULTIPLAYER_TEAM_H
#define MULTIPLAYER_TEAM_H

/* multiplayer_team — the team a player belongs to in a multiplayer game variant. This is the
 * GAME-MODE-SPLIT counterpart of game_team (game_team.h): the same fields (`team_index`,
 * object.owner_team_index) hold a game_team (campaign allegiance) value in single-player and a
 * multiplayer_team value in a multiplayer game engine. Which one applies is selected by game mode:
 *   - object_damage_* gate the game_team reading behind `!game_engine_running()` (single-player);
 *   - the multiplayer game-engine files (ctf_*, oddball_*, player_score, server_team_change) read
 *     this multiplayer_team value.
 *
 * Value range is proven from the binary: game_engine_rasterize_in_game_score clamps team_index to
 * [0,1] before indexing team_colors[], and server_team_change_from_network accepts only 0 or 1 — a
 * two-team space.
 *
 * GUESS (clearly marked): the two colors are named red=0, blue=1. Rationale: the binary carries no
 * "red"/"blue" strings for these (MP team colors live in tag/UI data not present in this build), so
 * the color-to-index mapping is not proven from this binary alone. red=0 / blue=1 is Halo's universal
 * convention (team_colors[0] is red, team_colors[1] is blue across the series' UI, scoreboard and game
 * variants), so the risk is low, but the specific color labels are an inference, not a binary fact.
 * The index values themselves (0 and 1) ARE proven. */
enum multiplayer_team {
	_multiplayer_team_red = 0,   /* GUESS: color label by Halo convention; index 0 is proven */
	_multiplayer_team_blue = 1,  /* GUESS: color label by Halo convention; index 1 is proven */
	NUMBER_OF_MULTIPLAYER_TEAMS = 2,
};

#endif /* MULTIPLAYER_TEAM_H */
