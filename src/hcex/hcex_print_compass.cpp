/* hcex_print_compass @0x823D3700 — hcex debug bridge: format one view's compass reading
 * ("compass[<idx>] = <magnitude>\n") into a temporary ref-counted string via dsSPrintf and append it to the
 * HCEX on-screen debug text via hcex_dbg_add_string, then release the temporary. dsTSTRING_flat / dsSPrintf are
 * ws-engine boundaries. */

#include "../headers/hcex/hcex_ds_boundary.h"

extern "C" void hcex_dbg_add_string(const char *string);

extern "C" void hcex_print_compass(int idx, float compass_magnitude)
{
    dsTSTRING_flat line;

    dsSPrintf(&line, "compass[%d] = %.3f\n", idx, compass_magnitude);
    hcex_dbg_add_string(line.pBuffer->str);

    if ( --line.pBuffer->refCount == 0 )
        dlFree(line.pBuffer);
}
