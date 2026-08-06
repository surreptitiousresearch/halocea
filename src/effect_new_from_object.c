/* effect_new_from_object @0x836E3F10 — allocate and initialize a live effect attached to an object,
 * binding its scale, color, optional impulse vector field, and marker-derived spawn locations.
 *
 * Deviation: PPC float-arg GPR-skip ABI. scale_a (f1) and scale_b (f2) are passed in FPRs but reserve
 * the matching GPR slots r7/r8, so the decompiler invented two phantom integer params (color/r7,
 * impulse_field/r8) between them and the real trailing args. The genuine arguments after the floats
 * are color (r9) and impulse_field (r10); the phantoms are dropped. Verified against disasm at
 * 0x836E3F38-0x836E3F3C (mr r30,r9 / mr r29,r10). */

#include <stdint.h>
#include "headers/effect_datum.h"
#include "headers/effect_vector_field.h"
#include "headers/data_array.h"
#include "headers/real_rgb_color.h"
#include "headers/effect_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/object_marker.h"
#include "headers/object_marker.h"
#include "headers/data_array.h"
extern void *memset(void *dst, int value, unsigned int n);

/* passed as the get_markers_by_name callback to effect_build_locations — they are functions,
   not char* data (prior recon mis-declared them; the __fastcall typedef was masking it) */
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

extern void *datum_get(data_array *data, int index);
extern int effect_allocate(int definition_index, int owner_object_index, uint8_t can_be_deterministic);
extern int16_t first_person_weapon_get_local_index(int object_index);
extern uint8_t effects_object_is_corpse(int object_index);
extern void effect_build_locations(effect_datum *effect, int16_t (*get_markers_by_name)(int, const char *, object_marker *, int16_t));
extern void hcex_init_effect_by_locations(int effect_index, int object_index, float scale);
extern void effect_update(int effect_index, float dt);

extern void impulse_effect_initialize(effect_datum *effect, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
int effect_new_from_object(
        int definition_index,
        int owner_object_index,
        int object_index,
        int16_t force_local_player_index,
        float scale_a,
        float scale_b,
        const real_rgb_color *color,
        const effect_vector_field *impulse_field)
{
    int effect_index = effect_allocate(definition_index, owner_object_index, 1u);
    if ( effect_index == -1 )
        return effect_index;

    /* recovered: effect_data->data + 252*(u16)effect_index -> datum_get (252 == effect datum_size incl. 2-byte salt header) */
    effect_datum *effect = datum_get(effect_data, effect_index);
    /* DEVIATION: collapsed verbatim-inlined body of impulse_effect_initialize@0x836E15E0 (zero-xref donor) back into a call; host mutated the `color` parameter in place for the default (functionally identical to donor's local ternary) and `color`/`impulse_field` are otherwise unused after this point, so passing them straight through is safe. */
    impulse_effect_initialize(effect, scale_a, scale_b, color, impulse_field);

    effect->object_index = object_index;
    effect->local_player_index = first_person_weapon_get_local_index(object_index);
    if ( effects_corpse_nonviolent && effects_object_is_corpse(effect->object_index) )
        effect->flags |= (1u << _effect_nonviolent_bit);

    memset(effect->location_datum_indices, -1, sizeof(effect->location_datum_indices));
    effect_build_locations(effect, object_get_marker_by_name);
    if ( (uint16_t)effect->local_player_index != 0xFFFF )
        effect_build_locations(effect, first_person_weapon_get_marker_by_name);
    if ( force_local_player_index != -1 )
        effect->local_player_index = force_local_player_index;

    hcex_init_effect_by_locations(effect_index, object_index, scale_a);
    effect_update(effect_index, 0.0);
    return effect_index;
}
