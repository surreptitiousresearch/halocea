#include "../headers/havok/hkSocket.h"

/* hkSocket::hkSocket — initialise the referenced-object header, install the socket
   vtable and the two stream-adapter vtables, point each adapter back at the owning
   socket, and run the one-time platform network-stack init (if a hook is set and it
   has not yet run). */
void hkSocket_construct(hkSocket *self)
{
    self->m_referenceCount = 1;
    self->__vftable = (hkSocket_vtbl *)&hkSocket_vftable;

    self->m_reader.m_referenceCount = 1;
    self->m_reader.m_socket = nullptr;
    self->m_reader.__vftable = (hkSocket_ReaderAdapter_vtbl *)&hkSocket_ReaderAdapter_vftable;

    self->m_writer.__vftable = (hkSocket_WriterAdapter_vtbl *)&hkSocket_WriterAdapter_vftable;
    self->m_writer.m_referenceCount = 1;
    self->m_writer.m_socket = nullptr;

    self->m_reader.m_socket = self;
    self->m_writer.m_socket = self;

    if (!s_platformNetInitialized.m_bool)
    {
        if (s_platformNetInit)
        {
            s_platformNetInit();
            s_platformNetInitialized.m_bool = 1;
        }
    }
}
