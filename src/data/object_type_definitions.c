/* object_type_definitions @ 0x84177C0C (.data, 48 bytes)
 * DB applied_types: object_type_definition *object_type_definitions[12];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x84177120 -> biped_data_definition
 *   +0x0004 = 0x841771E8 -> vehicle_data_definition
 *   +0x0008 = 0x84177378 -> weapon_data_definition
 *   +0x000C = 0x84177440 -> equipment_data_definition
 *   +0x0010 = 0x84177508 -> garbage_data_definition
 *   +0x0014 = 0x841775D0 -> projectile_data_definition
 *   +0x0018 = 0x84177698 -> scenery_data_definition
 *   +0x001C = 0x841778F0 -> machine_data_definition
 *   +0x0020 = 0x841779B8 -> control_data_definition
 *   +0x0024 = 0x84177A80 -> light_fixture_data_definition
 *   +0x0028 = 0x84177B48 -> placeholder_data_definition
 *   +0x002C = 0x84177760 -> sound_scenery_data_definition
 * data-init array of 12 pointers (48 bytes) to the
 * per-object-type descriptor structs, indexed by object type code. Pointer targets recovered from
 * the binary's relocated addresses (all named globals in the DB).
 */
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
