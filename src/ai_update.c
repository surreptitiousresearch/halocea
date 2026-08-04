/* ai_update @ 0x836EA3D0 — drive the AI for the tick. Only runs once AI is initialized for the map.
 * After placing any pending mounted-weapon AI, if AI is active it updates conversations, encounters and
 * actors (and marks that it now holds control data); if AI is inactive but still holds control data it
 * freezes the actors and clears the flag.
 *
 * Deviation: the decompiler threaded each call's return through the next argument (v0..v2), a PPC
 * register-reuse artifact — these updates take no argument. Field offsets verified against disassembly:
 * ai_active @0x00, ai_initialized_for_map @0x01, ai_has_control_data @0x02. */

#include "headers/ai_globals.h"

extern void ai_place_pending_mounted_weapons(void);
extern void ai_conversation_update(void);
extern void encounters_update(void);
extern void actors_update(void);
extern void actors_freeze(void);

void ai_update(void)
{
    if ( !ai_globals->ai_initialized_for_map )
        return;

    ai_place_pending_mounted_weapons();

    if ( ai_globals->ai_active )
    {
        ai_conversation_update();
        encounters_update();
        actors_update();
        ai_globals->ai_has_control_data = 1;
    }
    else
    {
        if ( !ai_globals->ai_has_control_data )
            return;
        actors_freeze();
        ai_globals->ai_has_control_data = 0;
    }
}
