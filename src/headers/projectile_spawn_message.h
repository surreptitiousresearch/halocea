#pragma once
/* _projectile_spawn_message — the decoder TU's legacy local name for the projectile-spawn wire
 * payload. The body is the DB type projectile_new_data and lives in its own header; this file
 * used to carry a second file-scope copy of it (identical members, pads spelled _pad0A/_pad51),
 * which is what made the header-layout probe emit `redefinition of 'projectile_new_data'`. */

#include "projectile_new_data.h"

/* legacy local name for the same wire payload (decoder TU) */
typedef projectile_new_data _projectile_spawn_message;
