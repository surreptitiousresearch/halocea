#pragma once
/* game_engine — the multiplayer game-mode dispatch table (game_engine.c). A pointer to one of these is
 * installed per game variant (slayer, ctf, etc.); each member is a callback the engine invokes at the
 * matching lifecycle point. Full 156-byte layout from the database. */

#include <stdint.h>
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
    void          (*dispose)(void);                         /* 0x08 */
    uint8_t (*initialize)(void);                            /* 0x0C */
    void          (*dispose_from_old_map)(void);            /* 0x10 */
    void          (*player_added)(int);                     /* 0x14 */
    void          (*player_removed)(int);                   /* 0x18 */
    void          (*game_ending)(void);                     /* 0x1C */
    void          (*game_starting)(void);                   /* 0x20 */
    void          (*statistics_append)(game_statistics *, game_statistics *); /* 0x24 */
    void          (*handle_client_message)(int, void *, int16_t); /* 0x28 */
    void          (*handle_server_message)(void *, int16_t); /* 0x2C */
    void          (*pregame_post_rasterize)(void);          /* 0x30 */
    void          (*post_rasterize)(void);                  /* 0x34 */
    void          (*player_update)(int);                    /* 0x38 */
    void          (*weapon_update)(int, weapon_datum *);    /* 0x3C */
    uint8_t (*weapon_pickup)(int, int);                     /* 0x40 */
    void          (*weapon_drop)(int);                      /* 0x44 */
    void          (*update)(void);                          /* 0x48 */
    int           (*get_score)(int, get_score_type);        /* 0x4C */
    int           (*get_team_score)(int);                   /* 0x50 */
    uint16_t *(*get_score_string)(int, uint16_t *);         /* 0x54 */
    uint16_t *(*get_score_header_string)(uint16_t *);       /* 0x58 */
    uint16_t *(*get_team_score_string)(int, uint16_t *);    /* 0x5C */
    uint8_t (*allow_pick_up)(int, int);                     /* 0x60 */
    void          (*player_damaged_player)(int, int, uint8_t); /* 0x64 */
    void          (*player_killed_player)(int, int, int, uint8_t); /* 0x68 */
    uint8_t (*rasterize_score)(int, int, int, uint16_t *, int); /* 0x6C */
    float         (*starting_location_rating)(int, scenario_player *); /* 0x70 */
    void          (*prespawn_player_update)(int);           /* 0x74 */
    uint8_t (*postspawn_player_update)(int);                /* 0x78 */
    int           (*game_engine_player_get_team_index)(int); /* 0x7C */
    uint8_t (*goal_matches_player)(int, int);               /* 0x80 */
    uint8_t (*game_engine_test_flag)(int);                  /* 0x84 */
    uint8_t (*game_engine_test_trait)(int, int);            /* 0x88 */
    int           (*game_engine_did_player_win)(int);       /* 0x8C */
    void          (*replicate_game_mode_state_to_network)(message_delta_processor_mode, const int); /* 0x90 */
    void          (*replicate_game_mode_state_from_network)(message_delta_processor_header *, network_game_client *); /* 0x94 */
    void          (*player_changed_team)(int, uint8_t);     /* 0x98 */
};                                              /* 156 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern struct game_engine *game_engine;

#ifdef __cplusplus
}
#endif
