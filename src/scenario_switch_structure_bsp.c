/* scenario_switch_structure_bsp @ 0x83703AA0 — switch the active structure BSP: run the
 * 10 disconnect procs, unload the old BSP, load the new one, rebind the global
 * structure/collision/bsp3d pointers, then run the 13 reconnect procs. Returns 1 on a
 * successful load. Raw tag-instance address math reproduced from the decompiler. */
#include <stdint.h>
#include "headers/scenario.h"

#include "headers/scenario_structure_bsp_reference.h"
#include "headers/structure_bsp.h"
#include "headers/scenario_globals.h"
#include "headers/global_tag_instances.h"  /* global_tag_instances; base_address@0x14 */
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler's inline structure_bsp_reference / structure_bsp / scenario_global_data /
 * global_tag_instances models are superseded by the DB-verified headers (structure_bsp.index@0x1C,
 * collision_bsp@0xB0, structure_bsp_index@0, base_address@0x14). */

typedef struct collision_bsp collision_bsp;
typedef struct bsp3d bsp3d;


extern void hcex_scenario_switch_structure_bsp(int16_t);
extern void main_stop_time(void);
extern void main_start_time(void);
extern void (*disconnect_from_structure_bsp_procs[10])(void);
extern void (*reconnect_to_structure_bsp_procs[13])(void);
extern void scenario_structure_bsp_unload(const scenario_structure_bsp_reference *reference);
extern uint8_t scenario_structure_bsp_load(const scenario_structure_bsp_reference *reference);

uint8_t scenario_switch_structure_bsp(int16_t structure_bsp_index)
{
    scenario_structure_bsp_reference *ref;
    char reconnect_needed = 0;

    if ( structure_bsp_index == global_structure_bsp_index
      || structure_bsp_index < 0
      || structure_bsp_index >= global_scenario->structure_bsp_references.count )
        return 0;

    /* recovered: (char *)address + 32*idx -> typed element pointer */
    ref = (scenario_structure_bsp_reference *)global_scenario->structure_bsp_references.address
          + structure_bsp_index;
    hcex_scenario_switch_structure_bsp(structure_bsp_index);
    /* DEVIATION: the decompiler threaded main_stop_time()'s r3 as `s` through the disconnect
     * loop; the handlers are void(void) and `s` is never read after the loop, so it is dropped. */
    main_stop_time();

    if ( (uint16_t)global_structure_bsp_index != 0xFFFF )
    {
        int i;
        for ( i = 0; i < 10; ++i )
            disconnect_from_structure_bsp_procs[i]();
        reconnect_needed = 1;
        scenario_structure_bsp_unload(
            (scenario_structure_bsp_reference *)global_scenario->structure_bsp_references.address
            + global_structure_bsp_index);
        scenario_globals->structure_bsp_index = -1;
        global_structure_bsp_index = -1;
    }

    if ( scenario_structure_bsp_load(ref) )
    {
        global_structure_bsp = TAG_GET(structure_bsp, ref->structure_bsp.index);
        global_collision_bsp = (collision_bsp *)global_structure_bsp->collision_bsp.address;
        global_bsp3d = (bsp3d *)global_structure_bsp->collision_bsp.address;
        scenario_globals->structure_bsp_index = structure_bsp_index;
        global_structure_bsp_index = structure_bsp_index;
        if ( reconnect_needed )
        {
            int i;
            for ( i = 0; i < 13; ++i )
                reconnect_to_structure_bsp_procs[i]();
        }
        main_start_time();
        return 1;
    }
    main_start_time();
    return 0;
}
