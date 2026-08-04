#pragma once
/* projectile_definition — the 'proj' tag: a base object definition (object @0x000) followed by the
 * projectile-specific definition (projectile @0x17C). Composite layout from the database (588 bytes). */

#include "_object_definition.h"
#include "_projectile_definition.h"

typedef struct projectile_definition
{
    _object_definition     object;        /* 0x000 */
    _projectile_definition projectile;    /* 0x17C */
} projectile_definition;                  /* 0x24C = 588 bytes */
