#pragma once
#include "hkSocket.h"

/* hkBsdSocket, size 36 (DB types_members). */
typedef struct hkBsdSocket
{
    hkSocket base;                    /* 0  */
    unsigned int m_socket;            /* 32 */
} hkBsdSocket;

void hkBsdSocket_construct(hkBsdSocket *self, unsigned int socket); /* hkBsdSocket::hkBsdSocket */
void hkBsdSocket_close(hkBsdSocket *self);                          /* hkBsdSocket::close */
