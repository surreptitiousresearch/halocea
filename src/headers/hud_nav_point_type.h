#pragma once
/* HUD nav-point type/kind enumerations (DB anonymous enums).
 *
 * Nav-point type — DB type _1FD370FFD1BC21133536E9DCF7FA075B. This is the value stored in the
 * signed 4-bit hud_nav_point_datum::type field (0xF/-1 == deactivated). */
enum hud_nav_point_type
{
    _nav_point_flag             = 0,
    _nav_point_object           = 1,
    _nav_point_game_engine_flag = 2,
    NUMBER_OF_NAV_POINT_TYPES   = 3
};

/* Per-player active nav-point slot count — DB type _C7605301681E0C8DAEEB5543875CE3CD. */
enum
{
    MAXIMUM_ACTIVE_NAV_POINTS = 4
};
