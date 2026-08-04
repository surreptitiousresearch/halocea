#pragma once

#include "actor_optional_prop.h"

/* actor_prop_environment @ DB type (1540 bytes) — the working set of perception props an actor is tracking on
 * one side (enemies or friends): a count of already-existing (acknowledged) props plus a bounded list of
 * optional candidates awaiting promotion. */
typedef struct actor_prop_environment
{
    __int16             existing_count;          /* 0x000 */
    __int16             optional_count;          /* 0x002 */
    actor_optional_prop optional_props[128];     /* 0x004 */
} actor_prop_environment;
