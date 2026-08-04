/* projectile_export_function_values @0x83759D10 — evaluate the projectile tag's four export-function
 * selectors (definition->projectile.function_modes; 0 = none stops the scan) and store each result into
 * the projectile object's export slots (object.incoming_function_values): 1 = range remaining /
 * definition range, 2 = time remaining (detonation_timer), 3 = 1.0 while tracer flag (projectile flags
 * bit 1) is set. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/projectile_export_function_mode.h"
#include "headers/projectile_datum_flags.h"
#include "headers/blam_data_globals.h"


void projectile_export_function_values(int projectile_index)
{
    projectile_datum *projectile =
        (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    projectile_definition *definition =
        TAG_GET(projectile_definition, projectile->definition_index);

    int export_index = 0;
    do
    {
        if ( !definition->projectile.function_modes[export_index] )
            goto next_export;

        unsigned __int16 function_selector = definition->projectile.function_modes[export_index];
        float value = 0.0f;
        switch ( function_selector )
        {
        case _projectile_export_range_remaining:
            if ( definition->projectile.detonation_maximum_range == 0.0f )
                value = 0.0f;
            else
                value = projectile->projectile.odometer / definition->projectile.detonation_maximum_range;
            break;
        case _projectile_export_time_remaining:
            value = projectile->projectile.detonation_timer;
            break;
        case _projectile_export_tracer:
            if ( (projectile->projectile.flags & (1u << _projectile_tracer_bit)) != 0 )
                value = 1.0f;
            break;
        }
        projectile->object.incoming_function_values[export_index] = value;

next_export:
        export_index = (__int16)(export_index + 1);
    }
    while ( export_index < 4 );
}
