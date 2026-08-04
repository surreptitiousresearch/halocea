/* hcex_dump_scenery @0x83682910 — hcex debug bridge: dump every placed object of the scenery-/device-like
 * object types (vehicle, scenery, machine, control, light_fixture) through hcex_dump_scenery_by_type, then
 * flush the accumulated listing with a terminating hcex_scenery_output(0,0,NULL,NULL,NULL,NULL) call.
 * (vehicle is included so seat-like placements — chairs/cryotubes — are reported; see hcex_dump_scenery_by_type.)
 * The literal type values are object_type enum members (DB $F5AF3705676D16CDF71575CA63411CB1). */

#include "../headers/hcex/hcex_float3.h"
#include "../headers/object_type.h"

extern void hcex_dump_scenery_by_type(int object_type);
extern void hcex_scenery_output(int obj_id, int obj_type, const char *model_name, hcex_float3 *pos,
        hcex_float3 *fw, hcex_float3 *up);

extern "C" void hcex_dump_scenery(void)
{
    hcex_dump_scenery_by_type(object_type_vehicle);
    hcex_dump_scenery_by_type(object_type_scenery);
    hcex_dump_scenery_by_type(object_type_machine);
    hcex_dump_scenery_by_type(object_type_control);
    hcex_dump_scenery_by_type(object_type_light_fixture);
    hcex_scenery_output(0, 0, (const char *)0, (hcex_float3 *)0, (hcex_float3 *)0, (hcex_float3 *)0);
}
