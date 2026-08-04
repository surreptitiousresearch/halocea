/* hcex_obj_term_all @0x823EE900 — tear down every live HCEX bridge object: repeatedly terminate the first
 * entry of the hcexObjects vector (each termination removes it) until the vector is empty.
 *
 * hcex-bridge code; the ws-engine container is accessed through its data pointer. hcex_obj_term_base is the
 * hcex-bridge object teardown helper (reversed separately). Deviation: debug-only STRONG_ASSERT2 elided. */

#include "../headers/hcex/hcex_object_vector.h"

extern hcex_object_vector hcexObjects;
extern void hcex_obj_term_base(unsigned long id, int flag);

void hcex_obj_term_all(void)
{
    if ( hcexObjects.nElem )
    {
        do
            hcex_obj_term_base(hcexObjects.pData->id, 1);
        while ( hcexObjects.nElem );
    }
}
