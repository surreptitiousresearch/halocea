#ifndef ACTOR_PALETTE_ENTRY_H
#define ACTOR_PALETTE_ENTRY_H

/* actor_palette_entry — one entry of the scenario ai_actor_palette block: a tag_reference
 * to an actor ('actr') tag. Layout from the database. */

#include "tag_reference.h"

typedef struct actor_palette_entry
{
    tag_reference   reference;       /* 0x0 */
} actor_palette_entry;               /* 16 bytes */

#endif /* ACTOR_PALETTE_ENTRY_H */
