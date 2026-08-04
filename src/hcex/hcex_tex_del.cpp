/* hcex_tex_del @ 0x823F0AE0
   Releases a texture back to the engine texture manager, unless bit 5 of its
   state word marks it as engine-owned/undeletable. txmTEXTURE::Release is a
   txm* boundary method. */

#include <stdint.h>
#include "../headers/ws/txm/txmTEXTURE.h" /* typed: state @0x78 */

extern int txmTEXTURE_Release(txmTEXTURE *self);   /* txmTEXTURE::Release(void) */

extern "C" void hcex_tex_del(txmTEXTURE *tex)
{
    /* typed member (txmTEXTURE::state, apSTATE_T<unsigned long> @0x78; was a raw offset read) */
    if ( ((tex->state.state >> 5) & 1) == 0 )
        txmTEXTURE_Release(tex);
}
