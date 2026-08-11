// FUNCTION_INDEX entry: hkdExceptionReporter_DebugText_destruct @0x8386E968 (??1DebugText@hkdExceptionReporter@@QAA@XZ)
#include "../headers/havok/hkdExceptionReporter_DebugText.h"
#include "../headers/havok/hkThreadMemory.h"

/* hkdExceptionReporter::DebugText::~DebugText
   Destroys the embedded hkString. The only owned resource is the string buffer;
   it is freed only when the array owns its storage (top bit of m_capacityAndFlags
   clear -> value >= 0), the low 30 bits giving the allocated capacity. */
void hkdExceptionReporter_DebugText_destruct(hkdExceptionReporter_DebugText *self)
{
    int capacityAndFlags = self->m_text.m_string.m_capacityAndFlags;

    if (capacityAndFlags >= 0)
        hkThreadMemory_deallocateChunk(hkThreadMemory_getCurrent(),
                                       self->m_text.m_string.m_data,
                                       capacityAndFlags & 0x3FFFFFFF,
                                       HK_MEMORY_CLASS_ARRAY);
}
