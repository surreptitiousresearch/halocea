#pragma once

/* get_score_type — selects which score a game_engine's get_score callback computes. */
typedef enum get_score_type
{
    _get_score_individual = 0,
    _get_score_team = 1,
} get_score_type;
