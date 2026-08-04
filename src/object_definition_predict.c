/* object_definition_predict @0x836ECB40 — preload the predicted resources (tag data +0x170 block) for an
 * object definition tag. */

#include "headers/global_tag_instances.h"
#include "headers/tag_block.h"
#include "headers/_object_definition.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
extern void predicted_resources_precache(const tag_block *predicted_resources);

void object_definition_predict(int definition_index)
{
    if ( definition_index != -1 )
    {
        _object_definition *definition = TAG_GET(_object_definition, definition_index);
        predicted_resources_precache(&definition->predicted_resources);
    }
}
