/* rasterizer_dx9_shaders_load_from_ascii @ 0x836A1AB8
   Compiles every pixel-shader effect from its "D:\shaders\fx\<name>.fx" source
   into the shared effect pool, initializing each as it goes. On any failure it
   unloads all effects and reports failure. Iterates the full 123-entry table
   (the binary bounds the loop with the adjacent shader_bin_handle global). */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/d3dx_boundary.h"        /* ID3DXBuffer */
#include "headers/rasterizer_engine_globals.h" /* global_d3d_device */

extern int sprintf_0(char *string, const char *format, ...);
extern uint8_t rasterizer_dx9_shaders_init_effect(int index);
extern void rasterizer_dx9_shaders_unload_effects(void);

/* attest: uint8_t return — caller rasterizer_dx9_shaders_initialize normalizes with
   clrlwi r11,r3,24 @ 0x836A2470; the succeeded flag is kept byte-normalized (clrlwi
   r30 @ 0x836A1BB4) and returned via mr r3,r30. */
uint8_t rasterizer_dx9_shaders_load_from_ascii(void)
{
    uint8_t succeeded = 1;
    char path[352];

    for ( int index = 0; index < 123; ++index )
    {
        rasterizer_dx9_shader *entry = &shader_table[index];
        entry->effect = 0;

        ID3DXBuffer *errors = 0;
        sprintf_0(path, "%s%s.fx", "D:\\shaders\\fx\\", entry->filename);
        int created = D3DXCreateEffectFromFileA(global_d3d_device, path, global_effect_macros,
                                                0, 0, global_effect_pool, &entry->effect, &errors) >= 0;
        if ( errors )
            errors->lpVtbl->Release(errors);  /* opaque COM: call through vtbl (matches sibling files) */

        if ( !created || !rasterizer_dx9_shaders_init_effect(index) )
        {
            succeeded = 0;
            break;
        }
    }

    if ( !succeeded )
        rasterizer_dx9_shaders_unload_effects();
    return succeeded;
}
