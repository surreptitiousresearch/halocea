#pragma once

/* bink_movie — the movie index passed to attract_mode_get_localized_movie_path (the DB
 * prototype keeps the parameter a plain __int16; this enum names its domain). Source:
 * types_enum_values $BC15C09D07FC5118D8653BB6810ED46A (DB-verified). Value 3 is named
 * twice in the DB (the attract-movie count and the intro movie) — both kept verbatim.
 * Note: _bink_outro_movie (4) resolves to "bink\credits%s.bik" on disk — the outro IS the
 * credits movie; the DB name and the filename are both authentic. */
enum bink_movie {
	_bink_attract1_movie = 0,
	_bink_attract2_movie = 1,
	_bink_attract3_movie = 2,
	NUMBER_OF_ATTRACT_MODE_MOVIES = 3,
	_bink_intro_movie = 3,
	_bink_outro_movie = 4,
	_bink_teaser_movie = 5,
	NUMBER_OF_BINK_MOVIES = 6,
};

// bink_movie: DB-verified via types_enum_values $BC15C09D07FC5118D8653BB6810ED46A (anonymous compiled enum, names verbatim)
