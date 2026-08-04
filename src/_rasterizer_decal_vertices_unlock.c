#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_lock_operation.h"

void _rasterizer_decal_vertices_unlock(void)
{
    rasterizer_globals.current_lock_operation = _rasterizer_lock_none;
}
