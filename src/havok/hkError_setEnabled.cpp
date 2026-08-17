/* ?setEnabled@hkDefaultError@@UAAXHVhkBool@@@Z @0x838EDB30 */
#include "../headers/havok/hkDefaultError.h"
#include <stdint.h>

/* hkDefaultError::setEnabled — enable or disable reporting for a single assert id.
   When enabled, the id is removed from the disabled set; when disabled, it is
   inserted with value 1.
   DEVIATION: the decompiler types the third parameter as hkBool* but tests its low
   byte directly ((_BYTE)enabled) — the hkBool is really passed by value in the
   pointer register, so the boolean is the low byte of the argument. */
void hkError_setEnabled(hkDefaultError *self, unsigned int id, hkBool *enabled)
{
    hkPointerMapBase *disabled = (hkPointerMapBase *)&self->m_disabledAssertIds.m_map;

    if ((unsigned char)(uintptr_t)enabled)
        hkPointerMapBase_remove(disabled, id);
    else
        hkPointerMapBase_insert(disabled, id, 1u);
}
