/* _field_type_locality_reference_position_size_calculator @0x8379CB88 */
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"


int _field_type_locality_reference_position_size_calculator(
        _field_properties_definition *const field_properties_definition)
{
    int lan_bits = 3 * gLOCALITY_BITS_PER_COMPONENT_FULL_LAN + 1;
    int internet_bits = 3 * gLOCALITY_BITS_PER_COMPONENT_FULL_INTERNET + 1;
    if ( lan_bits <= internet_bits )
        return internet_bits;
    return lan_bits;
}
