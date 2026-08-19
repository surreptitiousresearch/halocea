/* blip_size_get @0x837BCA30 */
#include "headers/blam_data_globals.h"
/* DEVIATION: disasm (837BCA30) shows direct lfsx f1, r9, r8; the decompiler incorrectly added
 * a double intermediate. Function simply indexes a float table. */

float blip_size_get(char size_type)
{
    return sizes[size_type];
}
