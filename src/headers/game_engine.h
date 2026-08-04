#pragma once
/* game_engine — the multiplayer game-mode dispatch table (game_engine.c). A pointer to one of these is
 * installed per game variant (slayer, ctf, etc.); each member is a callback the engine invokes at the
 * matching lifecycle point. Full 156-byte layout from the database. */

#include "get_score_type.h"

typedef struct game_statistics game_statistics;
typedef struct weapon_datum weapon_datum;
typedef struct scenario_player scenario_player;
typedef struct message_delta_processor_header message_delta_processor_header;
typedef struct network_game_client network_game_client;
typedef enum message_delta_processor_mode message_delta_processor_mode;

struct game_engine
{
    const char     *name;                                   /* 0x00 */
    unsigned int    type;                                   /* 0x04 */
    void          (__fastcall *dispose)(void);              /* 0x08 */
    unsigned __int8 (__fastcall *initialize)(void);         /* 0x0C */
    void          (__fastcall *dispose_from_old_map)(void); /* 0x10 */
    void          (__fastcall *player_added)(int);          /* 0x14 */
    void          (__fastcall *player_removed)(int);        /* 0x18 */
    void          (__fastcall *game_ending)(void);          /* 0x1C */
    void          (__fastcall *game_starting)(void);        /* 0x20 */
    void          (__fastcall *statistics_append)(game_statistics *, game_statistics *); /* 0x24 */
    void          (__fastcall *handle_client_message)(int, void *, __int16);             /* 0x28 */
    void          (__fastcall *handle_server_message)(void *, __int16);                  /* 0x2C */
    void          (__fastcall *pregame_post_rasterize)(void); /* 0x30 */
    void          (__fastcall *post_rasterize)(void);         /* 0x34 */
    void          (__fastcall *player_update)(int);           /* 0x38 */
    void          (__fastcall *weapon_update)(int, weapon_datum *); /* 0x3C */
    unsigned __int8 (__fastcall *weapon_pickup)(int, int);    /* 0x40 */
    void          (__fastcall *weapon_drop)(int);             /* 0x44 */
    void          (__fastcall *update)(void);                 /* 0x48 */
    int           (__fastcall *get_score)(int, get_score_type); /* 0x4C */
    int           (__fastcall *get_team_score)(int);          /* 0x50 */
    unsigned __int16 *(__fastcall *get_score_string)(int, unsigned __int16 *);        /* 0x54 */
    unsigned __int16 *(__fastcall *get_score_header_string)(unsigned __int16 *);       /* 0x58 */
    unsigned __int16 *(__fastcall *get_team_score_string)(int, unsigned __int16 *);    /* 0x5C */
    unsigned __int8 (__fastcall *allow_pick_up)(int, int);    /* 0x60 */
    void          (__fastcall *player_damaged_player)(int, int, unsigned __int8);      /* 0x64 */
    void          (__fastcall *player_killed_player)(int, int, int, unsigned __int8);  /* 0x68 */
    unsigned __int8 (__fastcall *rasterize_score)(int, int, int, unsigned __int16 *, int); /* 0x6C */
    float         (__fastcall *starting_location_rating)(int, scenario_player *);      /* 0x70 */
    void          (__fastcall *prespawn_player_update)(int);  /* 0x74 */
    unsigned __int8 (__fastcall *postspawn_player_update)(int); /* 0x78 */
    int           (__fastcall *game_engine_player_get_team_index)(int);               /* 0x7C */
    unsigned __int8 (__fastcall *goal_matches_player)(int, int); /* 0x80 */
    unsigned __int8 (__fastcall *game_engine_test_flag)(int);   /* 0x84 */
    unsigned __int8 (__fastcall *game_engine_test_trait)(int, int); /* 0x88 */
    int           (__fastcall *game_engine_did_player_win)(int); /* 0x8C */
    void          (__fastcall *replicate_game_mode_state_to_network)(message_delta_processor_mode, const int); /* 0x90 */
    void          (__fastcall *replicate_game_mode_state_from_network)(message_delta_processor_header *, network_game_client *); /* 0x94 */
    void          (__fastcall *player_changed_team)(int, unsigned __int8); /* 0x98 */
};                                              /* 156 bytes */

extern struct game_engine *game_engine;
