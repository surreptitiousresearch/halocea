#ifndef GAME_TEAM_H
#define GAME_TEAM_H

/* game_team — the campaign/AI allegiance a squad or encounter belongs to (the "team" a scenario
 * encounter is assigned). Distinct from the multiplayer red/blue team index (also spelled *team_index*
 * but only ever 0/1 in MP contexts) — do NOT use these names for object.owner_team_index / player.team_index.
 *
 * NOT in headers_ref/DB as an enum (the member is a plain __int16). Names recovered from the binary's
 * own data: one of the four HaloScript-facing enums registered in the hs enum-type block @0x8211C9C4;
 * its ordered name-string table is @0x8417D40C:
 *   default[0] player[1] human[2] covenant[3] flood[4] sentinel[5]  (6..9 = unused). */
/* layout bound to DB enum: types_enum_values _5D2EF8FA442E585DBFF707A5F144D58C */
enum game_team {
	_game_team_default = 0x0,
	_game_team_player = 0x1,
	_game_team_human = 0x2,
	_game_team_covenant = 0x3,
	_game_team_flood = 0x4,
	_game_team_sentinel = 0x5,
	_game_team_unused6 = 0x6,
	_game_team_unused7 = 0x7,
	_game_team_unused8 = 0x8,
	_game_team_unused9 = 0x9,
	NUMBER_OF_SOLO_CAMPAIGN_TEAMS = 0xA,
	_team_red = 0x0,
	_team_blue = 0x1,
	MAXIMUM_MULTIPLAYER_GAME_TEAMS = 0x2,
};

#endif /* GAME_TEAM_H */
