/* object_destroy @0x836B2D38 — fully destroy an object: fire the HCEX grunt-explode effect for grunts,
 * deplete the body, spawn the collision model's body-destroyed effect, notify children, and delete the
 * object.
 *
 * Deviation: the effect_new_from_object color/impulse args are NULL (r9=r10=0, verified at 0x836B2DFC);
 * the decompiler's computed color pointer and 0x82000000 impulse are float-skip phantoms. The inlined
 * byte-compare loop is strcmp. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/collision_model.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"

#include "headers/real_rgb_color.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern char *tag_get_name(int16_t tag_index);
extern void hcex_init_effect_simple(const char *eff_name, const real_point3d *point, const real_vector3d *vector);
extern void object_deplete_body(int object_index);
typedef struct effect_vector_field effect_vector_field;
extern int effect_new_from_object(int definition_index, int owner_object_index, int object_index, int16_t force_local_player_index, float scale_a, float scale_b, const real_rgb_color *color, const effect_vector_field *impulse_field);
extern void object_destroy_notify_children(int object_index);
extern void object_delete(int object_index);

void object_destroy(int object_index)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *definition = TAG_GET(_object_definition, object->definition_index);

    char *name = tag_get_name(object->definition_index);
    if ( name && strcmp(name, "characters\\grunt\\grunt") == 0 )
        hcex_init_effect_simple("effects\\grunt_explode", &object->object.position,
                                &object->object.forward);

    object_deplete_body(object_index);

    int collision_model_index = definition->collision_model.index;
    if ( collision_model_index != -1 )
        effect_new_from_object(
            TAG_GET(collision_model, collision_model_index)->resistance.body_destroyed_effect.index,
            object_index, object_index, -1, 0.0, 0.0, 0, 0);

    object_destroy_notify_children(object_index);
    object_delete(object_index);
}
