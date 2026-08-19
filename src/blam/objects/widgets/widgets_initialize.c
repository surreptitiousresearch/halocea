/* widgets_initialize @ 0x83755FB8 — widget pool + per-type initializers (5 types) */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/widget_type_definition.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler modeled widget_type_definition as a single {initialize@0} pointer; the
 * DB-verified vtable (widget_type_definition.h) has initialize@8 with a 0x28-byte stride — disasm reads
 * `lwz r11, 8(...)`. Reusing the canonical header corrects the field offset and array stride. */

extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void widgets_initialize(void)
{
    int i;

    /* disasm: r3 is stored to widget_data then clobbered by the bctrl loop and never reloaded
     * before blr — no deterministic return value (0 consumers) => void */
    widget_data = game_state_data_new("widget", 64, 12);
    for ( i = 0; i < 5; ++i )
    {
        void (*initialize)(void) = widget_type_definitions[i].initialize;
        if ( initialize )
            initialize();   /* disasm: bctrl, return value unused (void hook) */
    }
}
