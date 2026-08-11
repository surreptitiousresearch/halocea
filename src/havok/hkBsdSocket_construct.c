/* FUNCTION_INDEX entry: hkBsdSocket_construct @0x838EE3E0 (??0hkBsdSocket@@QAA@I@Z) */
#include "../headers/havok/hkBsdSocket.h"

/* boundary — base ctor, vtable, close, and the Berkeley sockets API. */
extern void hkSocket_construct(hkSocket *self);           /* hkSocket::hkSocket */
extern void hkBsdSocket_close(hkBsdSocket *self);         /* hkBsdSocket::close */
extern int  socket(int af, int type, int protocol);      /* Winsock/BSD */
extern void *const hkBsdSocket_vftable;                   /* hkBsdSocket::`vftable' */

/* hkBsdSocket::hkBsdSocket — wrap an existing socket handle, or, when given the
   invalid handle (-1), close and create a fresh AF_INET(2) SOCK_STREAM(1)
   socket. */
void hkBsdSocket_construct(hkBsdSocket *self, unsigned int s)
{
    hkSocket_construct(&self->base);
    self->m_socket = s;
    self->base.__vftable = (hkSocket_vtbl *)&hkBsdSocket_vftable;
    if (s == (unsigned int)-1)
    {
        hkBsdSocket_close(self);
        self->m_socket = (unsigned int)socket(2, 1, 0);
    }
}
