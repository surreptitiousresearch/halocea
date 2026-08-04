/* Bound to DB enum types_enum_values _D93FA080628339FD8C340CAA12AAECF2 (2026-07-20 BLOCKED-retry). */
#ifndef _POSTGAME_STATISTIC_H_
#define _POSTGAME_STATISTIC_H_

/* postgame_statistic — the "which stat to sort/populate by" selector (populate_statistic_buffer /
 * select_players_to_display). Previously an EMPTY enum shell (DB resolved no enumerators); the IDA DB
 * now carries the compiled values. DB-AUTHORITATIVE. */
enum postgame_statistic // 4 bytes
{
	_postgame_statistic_ranking = 0,
	_postgame_statistic_custom = 1,
	_postgame_statistic_kills = 2,
	_postgame_statistic_assists = 3,
	_postgame_statistic_deaths = 4,
};

#endif /* _POSTGAME_STATISTIC_H_ */
