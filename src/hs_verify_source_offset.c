/* hs_verify_source_offset @0x83775D68 */
#include "headers/hs_compile_globals.h"

int hs_verify_source_offset(int source_offset)
{
    if ( source_offset < 0 || source_offset >= hs_compile_globals.compiled_source_size )
    {
        hs_compile_globals.__noop = "bad source offset (you need to recompile.)";
        return 0;
    }
    return 1;
}
