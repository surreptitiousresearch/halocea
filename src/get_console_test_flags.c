/* get_console_test_flags @ 0x837221E8 — build the hs-token-enumeration flag mask used when
 * resolving a console command, based on the current game mode (engine running, network
 * client/server), the active player's profile, and any caller-supplied extra flags. Higher
 * permission bits (0x200..0x4000) are progressively masked off so the console only enumerates
 * symbols the current context is allowed to use. */

#include <stdint.h>
#include "headers/player_profile.h"
#include "headers/button_preset_flags.h"  /* profile.flags bits: _profile_*_bit */
#include "headers/hs_console_flags.h"     /* DB $B9FE84D7: allow bits 0-6, disallow = bit+8 */

extern int16_t game_connection(void);
extern uint8_t game_engine_running(void);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);

uint16_t get_console_test_flags(uint16_t extra_flags)
{
    int connection = game_connection();
    int16_t base = 1;
    int16_t mode_flags;
    uint16_t flags;
    int result;

    if ( game_engine_running() )
    {
        mode_flags = (1u << _hs_console_allowed_default) | (1u << _hs_console_ingame) | (1u << (_hs_console_cheating + HS_CONSOLE_DISALLOW_SHIFT));
    }
    else
    {
        player_profile profile;
        player_ui_get_active_player_profile(0, &profile);
        if ( ((profile.flags >> _profile_finished_game_bit) & 1) == 0 )
            base = 8193;
        mode_flags = base | (1u << _hs_console_idle);
    }

    flags = mode_flags | (1u << _hs_console_ship_game);
    if ( connection == 1 )
        flags |= (1u << (_hs_console_listen_server + HS_CONSOLE_DISALLOW_SHIFT))
               | (1u << (_hs_console_dedicated_server + HS_CONSOLE_DISALLOW_SHIFT));   /* network server */
    else if ( connection == 2 )
        flags |= (1u << _hs_console_listen_server);                                    /* network client */

    result = flags | extra_flags;
    if ( (flags | extra_flags) & (1u << (_hs_console_allowed_default + HS_CONSOLE_DISALLOW_SHIFT)) )
        result = (flags | extra_flags) & ~(1u << _hs_console_allowed_default);

    /* each disallow bit (N+8) strips its paired allow bit N */
    if ( result & (1u << (_hs_console_listen_server + HS_CONSOLE_DISALLOW_SHIFT)) )    result &= ~(1u << _hs_console_listen_server);
    if ( result & (1u << (_hs_console_dedicated_server + HS_CONSOLE_DISALLOW_SHIFT)) ) result &= ~(1u << _hs_console_dedicated_server);
    if ( result & (1u << (_hs_console_ingame + HS_CONSOLE_DISALLOW_SHIFT)) )           result &= ~(1u << _hs_console_ingame);
    if ( result & (1u << (_hs_console_idle + HS_CONSOLE_DISALLOW_SHIFT)) )             result &= ~(1u << _hs_console_idle);
    if ( result & (1u << (_hs_console_cheating + HS_CONSOLE_DISALLOW_SHIFT)) )         result &= ~(1u << _hs_console_cheating);
    if ( result & (1u << (_hs_console_ship_game + HS_CONSOLE_DISALLOW_SHIFT)) )        result &= ~(1u << _hs_console_ship_game);

    return (uint16_t)result;
}
