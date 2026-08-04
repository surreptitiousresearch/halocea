/* biped_export_function_values @0x837AC670 — evaluate the biped tag's four export-function selectors
 * (definition words +780..+786; 0 = none stops the scan) and store each result into the biped object's
 * export slots (floats +292..+304): function 1 = clamped-to-[0,1] speed ratio, computed as the object's
 * velocity magnitude (floats +104/+108/+112) divided by (definition's max forward speed * 1/30 tick). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"

extern float __fsqrts(float x);

void biped_export_function_values(int biped_index)
{
    biped_datum *biped =
        (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    biped_definition *definition =
        TAG_GET(biped_definition, biped->definition_index);

    int export_index = 0;
    do
    {
        __int16 function_selector = definition->biped.function_modes[export_index];
        if ( function_selector )
        {
            float value = 0.0f;
            if ( function_selector == 1 )
            {
                float velocity_x = biped->object.translational_velocity.n[0];
                float velocity_y = biped->object.translational_velocity.n[1];
                float velocity_z = biped->object.translational_velocity.n[2];
                float speed = __fsqrts(velocity_x * velocity_x + velocity_y * velocity_y + velocity_z * velocity_z);
                value = speed / (definition->biped.flying_velocity * SECONDS_PER_TICK);

                if ( value < 0.0f )
                    value = 0.0f;
                else if ( value > 1.0f )
                    value = 1.0f;
            }
            biped->object.incoming_function_values[export_index] = value;
        }
        export_index = (__int16)(export_index + 1);
    }
    while ( export_index < 4 );
}
