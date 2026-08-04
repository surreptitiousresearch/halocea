/* biped_make_footstep @0x837B0384 — spawn the footstep material effect for a biped's foot contact point.
 * Skips if the contact point index is out of range, the biped's model has no footstep effect, or the biped
 * isn't visible to the material-effect system. Looks up the contact-point marker by name (biped definition
 * contact_points block, biped_contact_point.marker_name) and spawns the effect at the marker
 * position, flagging it as a player footstep when the biped is a local player.
 * DEVIATION: the decompiler dropped the is-player flag (r27) and the marker name math; recovered from
 * disassembly. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_marker.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/biped_contact_point.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

extern void *datum_try_and_get(const data_array *data, int index);
extern uint8_t material_effect_visible(const real_point3d *origin);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void material_effect_new_from_point(int effects_definition_index, int16_t effect_index, const real_point3d *origin, float scale, uint8_t is_player);

void biped_make_footstep(int biped_index, int16_t event_index, int16_t contact_point_index)
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);

    unsigned __int8 is_player = 0;
    player_datum *player = datum_try_and_get(player_data, biped->unit.player_index);
    if ( player && (unsigned __int16)player->local_player_index != 0xFFFF )
        is_player = 1;

    if ( contact_point_index < definition->biped.contact_points.count && definition->biped.material_effects.index != -1
      && material_effect_visible(&biped->object.bounding_sphere_center) )
    {
        object_marker marker;
        const char *marker_name = ((const biped_contact_point *)definition->biped.contact_points.address)[contact_point_index].marker_name;
        if ( object_get_marker_by_name(biped_index, marker_name, &marker, 1) )
            material_effect_new_from_point(definition->biped.material_effects.index, event_index, (const real_point3d *)&marker.matrix.n[3], 0.0f, is_player);
    }
}
