/* widgets_update @0x83756548 — per-frame tick of each registered widget type (5 types); calls each
 * type's update proc (if any) with the frame delta. */

#include <stdint.h>
#include "headers/widget_type_definition.h"
#include "headers/blam_data_globals.h"


/* attest: f1 arrives and is forwarded to the update procs via fmr only (no frsp / no double
 * round-trip at 0x83756564/0x8375658C) — the parameter is single-precision float, not double. */
void widgets_update(float dt)
{
    int i;

    for ( i = 0; i < 5; i = (int16_t)(i + 1) )
    {
        void (*update)(float) = widget_type_definitions[i].update;
        if ( update )
            update(dt);
    }
}
