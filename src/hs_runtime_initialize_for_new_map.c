/* hs_runtime_initialize_for_new_map @ 0x83690A20 — per-map HaloScript runtime setup:
 * spin up the bootstrap thread, evaluate every scenario global into the global datum
 * table, then launch the startup/continuous scripts.
 *
 * The decompiler accesses the hs_thread and hs_global tag structures through raw
 * offsets (no named struct recovered); those raw accesses are reproduced verbatim with
 * comments. hs_thread stride = 536, hs_global stride = 92, hs_script stride = 92.
 *
 * DEVIATION: the decompiler's datum_new + thread-field init block is a verbatim
 * compiler-inlined copy of hs_thread_new @0x8368CFF0 constant-folded for
 * (type=hs_thread_type_global_initialize, script_index=-1, so sleep_until folds to 0) —
 * emitted as the call the source had. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_global_designator.h"
#include "headers/hs_thread.h"
#include "headers/hs_global_datum.h"
#include "headers/hs_global_internal.h"
#include "headers/hs_script.h"
#include "headers/scenario.h"

extern void data_make_valid(data_array *data);
extern short hs_external_global_count;

#include "headers/hs_runtime_globals.h"
#include "headers/hs_thread_type.h"
#include "headers/hs_type.h"
#include "headers/hs_thread_flags.h"
#include "headers/hs_script_type.h"
#include "headers/blam_data_globals.h"

extern int datum_new_at_index(data_array *data, int index);
extern void datum_delete(data_array *data, int index);
extern void hs_evaluate(int thread_index, int expression_index, int *destination);
extern void hs_thread_main(int thread_index);
extern int hs_global_evaluate(int16_t global_designator);
extern void object_list_add_reference(int object_list_index);
extern void hs_global_reconcile_write(int16_t global_designator);
extern int hs_thread_new(int16_t type, int script_index);

void hs_runtime_initialize_for_new_map(void)
{
    int boot_thread;

    data_make_valid(hs_thread_data);
    hs_runtime_globals.initialized = 1;
    hs_runtime_globals.executing_thread_index = -1;

    boot_thread = hs_thread_new(hs_thread_type_global_initialize, -1);

    if ( global_scenario_index != -1 )
    {
        scenario *scn = global_scenario;
        hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, boot_thread);
        int g;
        short index = 0;

        for ( g = 0; g < scn->hs_globals.count; ++g )
        {
            hs_global_internal *global_def = &((hs_global_internal *)scn->hs_globals.address)[g];
            int salted_index = HS_GLOBAL_IS_EXTERNAL(index)
                                   ? HS_GLOBAL_DESIGNATOR_TO_INDEX(g)
                                   : (hs_external_global_count + HS_GLOBAL_DESIGNATOR_TO_INDEX(g));
            int slot;

            datum_new_at_index(hs_global_data, salted_index | 0xACED0000);
            slot = HS_GLOBAL_DESIGNATOR_TO_INDEX(g);
            if ( !HS_GLOBAL_IS_EXTERNAL(index) )
                slot += hs_external_global_count;

            thread->script_index = -1;
            thread->stack->size = 0;
            /* evaluate the global's initializer expression into its datum value */
            hs_evaluate(boot_thread, global_def->initialization_expression_index,
                        &DATA_ARRAY_ELEMENT(hs_global_data, hs_global_datum, slot)->value.long_value);

            if ( (thread->flags & (1u << _hs_thread_in_function_call_bit)) != 0 )
            {
                hs_thread_main(boot_thread);
                if ( global_def->type == hs_type_object_list )
                    object_list_add_reference(hs_global_evaluate(index));
            }
            hs_global_reconcile_write(index);
            index = (short)(g + 1);
        }

        datum_delete(hs_thread_data, boot_thread);

        if ( scn->hs_scripts.count > 0 )
        {
            short s;
            for ( s = 0; s < scn->hs_scripts.count; ++s )
            {
                int type = ((hs_script *)scn->hs_scripts.address)[s].script_type;
                if ( type != _hs_script_static && type != _hs_script_stub )   /* launch */
                    hs_thread_new(hs_thread_type_script, s);
            }
        }
    }
}
