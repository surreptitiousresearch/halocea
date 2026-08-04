/* object_type_definitions @ 0x84177C0C — data-init array of 12 pointers (48 bytes) to the
 * per-object-type descriptor structs, indexed by object type code. Pointer targets recovered from
 * the binary's relocated addresses (all named globals in the DB). */
#include "../headers/object_type_definition.h"

extern object_type_definition biped_data_definition;          /* 0x84177120 */
extern object_type_definition vehicle_data_definition;         /* 0x841771E8 */
extern object_type_definition weapon_data_definition;          /* 0x84177378 */
extern object_type_definition equipment_data_definition;       /* 0x84177440 */
extern object_type_definition garbage_data_definition;         /* 0x84177508 */
extern object_type_definition projectile_data_definition;      /* 0x841775D0 */
extern object_type_definition scenery_data_definition;         /* 0x84177698 */
extern object_type_definition machine_data_definition;         /* 0x841778F0 */
extern object_type_definition control_data_definition;         /* 0x841779B8 */
extern object_type_definition light_fixture_data_definition;   /* 0x84177A80 */
extern object_type_definition placeholder_data_definition;     /* 0x84177B48 */
extern object_type_definition sound_scenery_data_definition;   /* 0x84177760 */

object_type_definition *object_type_definitions[12] =
{
    &biped_data_definition,
    &vehicle_data_definition,
    &weapon_data_definition,
    &equipment_data_definition,
    &garbage_data_definition,
    &projectile_data_definition,
    &scenery_data_definition,
    &machine_data_definition,
    &control_data_definition,
    &light_fixture_data_definition,
    &placeholder_data_definition,
    &sound_scenery_data_definition,
};
