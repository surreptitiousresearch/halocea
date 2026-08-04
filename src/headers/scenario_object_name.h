#pragma once
/* scenario_object_name — one element of scenario::object_names (tag_block, 36 bytes/entry). Layout
 * from the database `scenario_object_name` type. */

typedef struct scenario_object_name
{
    char     name[32];                       /* 0x00 */
    __int16  runtime_object_type;            /* 0x20 */
    __int16  runtime_scenario_datum_index;   /* 0x22 */
} scenario_object_name;                      /* 0x24 = 36 bytes */
