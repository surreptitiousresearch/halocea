/* ??1ErrorMessage@hkdExceptionReporter@@QAA@XZ @0x8386EF60 */
#include "../headers/havok/hkdExceptionReporter_ErrorMessage.h"
#include "../headers/havok/hkThreadMemory.h"

/* Per-element destructor for DebugText (it owns an hkString, so it is the only
   debug-primitive that needs element teardown before its backing array is freed).
   DEVIATION: the DB call is DebugText::`scalar deleting destructor'(elem, 0) — the
   0 flag means "destruct, do not free"; wrapped here as the plain destructor. */
extern void hkdExceptionReporter_DebugText_destruct(hkdExceptionReporter_DebugText *self); /* hkdExceptionReporter::DebugText::~DebugText */

/* hkdExceptionReporter::ErrorMessage::~ErrorMessage — release every owned array
   back to the current thread's allocator. Only arrays whose capacity flag has the
   sign bit clear (m_capacityAndFlags >= 0, i.e. heap-owned, not DONT_DEALLOCATE)
   are freed. The DebugText array is element-destructed first; the remaining debug
   arrays are POD and are freed directly; finally the description string buffer.
   Byte sizes match the DB element sizes (Point 32, Line 48, Triangle 64,
   Plane 48, Text 48, Frame 80).

   DEVIATION: the decompiler open-codes each capacity->byte-size conversion with
   shift/mask arithmetic against the low 30 capacity bits; reproduced here as
   `(cap & 0x3FFFFFFF) * elementSize` (equivalent, since the branch is only taken
   when the sign bit is clear). */
void hkdExceptionReporter_ErrorMessage_destruct(hkdExceptionReporter_ErrorMessage *self)
{
    hkThreadMemory *mem = hkThreadMemory_getCurrent();
    int cap, i, n;

    cap = self->m_frames.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_frames.m_data, (cap & 0x3FFFFFFF) * 80, HK_MEMORY_CLASS_ARRAY);

    n = self->m_texts.m_size;
    for (i = 0; i < n; ++i)
        hkdExceptionReporter_DebugText_destruct(&self->m_texts.m_data[i]);
    cap = self->m_texts.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_texts.m_data, (cap & 0x3FFFFFFF) * 48, HK_MEMORY_CLASS_ARRAY);

    cap = self->m_planes.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_planes.m_data, (cap & 0x3FFFFFFF) * 48, HK_MEMORY_CLASS_ARRAY);

    cap = self->m_triangles.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_triangles.m_data, (cap & 0x3FFFFFFF) * 64, HK_MEMORY_CLASS_ARRAY);

    cap = self->m_lines.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_lines.m_data, (cap & 0x3FFFFFFF) * 48, HK_MEMORY_CLASS_ARRAY);

    cap = self->m_points.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_points.m_data, (cap & 0x3FFFFFFF) * 32, HK_MEMORY_CLASS_ARRAY);

    cap = self->m_description.m_string.m_capacityAndFlags;
    if (cap >= 0)
        hkThreadMemory_deallocateChunk(mem, self->m_description.m_string.m_data, cap & 0x3FFFFFFF, HK_MEMORY_CLASS_ARRAY);
}
