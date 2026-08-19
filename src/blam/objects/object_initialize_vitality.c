/* object_initialize_vitality @0x836B1178 — set an object's body/shield health from its collision-model
 * tag (or caller-supplied overrides), then seed the current body/shield levels and alive flags.
 * Mythic-skull boost (hcex_mythic_skull) doubles biped body health (owner team 3/4), or raises a
 * vehicle's shield to match its body (team 5). */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/collision_model.h"
#include "headers/blam_data_globals.h"


void object_initialize_vitality(int object_index, const float *custom_body_vitality, const float *custom_shield_vitality)
{
    double body_vitality = 0.0;
    double shield_vitality = 0.0;
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);
    int collision_model_index = object_definition->collision_model.index;
    if ( collision_model_index != -1 )
    {
        collision_model *collision = TAG_GET(collision_model, collision_model_index);
        if ( collision )
        {
            body_vitality = collision->resistance.maximum_body_vitality;
            shield_vitality = collision->resistance.maximum_shield_vitality;
        }
    }

    if ( custom_body_vitality )
        body_vitality = *custom_body_vitality;
    if ( custom_shield_vitality )
        shield_vitality = *custom_shield_vitality;

    if ( hcex_mythic_skull && !object->object.type )
    {
        int owner_team = object->object.owner_team_index;
        if ( owner_team == 3 || owner_team == 4 )
            body_vitality = ((float)body_vitality * 2.0f);
        else if ( owner_team == 5 && shield_vitality < body_vitality )
            shield_vitality = body_vitality;
    }

    object->object.maximum_body_vitality = body_vitality;
    object->object.maximum_shield_vitality = shield_vitality;
    object->object.body_vitality = body_vitality <= 0.0 ? 0.0f : 1.0f;
    object->object.shield_vitality = shield_vitality <= 0.0 ? 0.0f : 1.0f;
}
