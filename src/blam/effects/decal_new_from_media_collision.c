/* decal_new_from_media_collision @0x83741398 — no-op (bare blr; confirmed via disasm). */

#include <stdint.h>
#include "headers/collision_result.h"
#include "headers/decal_editor_geometry.h"
#include "headers/real_vector3d.h"

void decal_new_from_media_collision(int decal_definition_index, const collision_result *collision, const real_vector3d *velocity, float radius_modifier, uint8_t permanent, int16_t forced_sequence_index, decal_editor_geometry *editor_geometry)
{
}
