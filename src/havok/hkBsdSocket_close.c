#include "../headers/havok/hkBsdSocket.h"

/* boundary — Winsock/BSD sockets teardown. */
extern int closesocket(unsigned int s); /* Win32 closesocket */

/* hkBsdSocket::close — close the underlying handle (if valid) and reset it to the
   invalid sentinel (-1). */
void hkBsdSocket_close(hkBsdSocket *self)
{
    unsigned int s = self->m_socket;
    if (s != (unsigned int)-1)
    {
        closesocket(s);
        self->m_socket = (unsigned int)-1;
    }
}
