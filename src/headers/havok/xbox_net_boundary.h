#pragma once
/* XDK secure-networking + Winsock boundary (platform APIs). */

/* size 13 — all one-byte config knobs. */
typedef struct XNetStartupParams
{
    unsigned char cfgSizeOfStruct;
    unsigned char cfgFlags;
    unsigned char cfgSockMaxDgramSockets;
    unsigned char cfgSockMaxStreamSockets;
    unsigned char cfgSockDefaultRecvBufsizeInK;
    unsigned char cfgSockDefaultSendBufsizeInK;
    unsigned char cfgKeyRegMax;
    unsigned char cfgSecRegMax;
    unsigned char cfgQosDataLimitDiv4;
    unsigned char cfgQosProbeTimeoutInSeconds;
    unsigned char cfgQosProbeRetries;
    unsigned char cfgQosSrvMaxSimultaneousResponses;
    unsigned char cfgQosPairWaitTimeInSeconds;
} XNetStartupParams;

/* Winsock WSAData — 400-byte platform struct (types_members WSAData); used as
   the WSAStartup output buffer. */
typedef struct WSAData
{
    unsigned __int16 wVersion;     /* 0x000 */
    unsigned __int16 wHighVersion; /* 0x002 */
    char szDescription[257];       /* 0x004 */
    char szSystemStatus[129];      /* 0x105 */
    unsigned __int16 iMaxSockets;  /* 0x186 */
    unsigned __int16 iMaxUdpDg;    /* 0x188 */
    unsigned char _pad18A[2];      /* 0x18A */
    char *lpVendorInfo;            /* 0x18C -> 0x190 (400) */
} WSAData;

int XNetStartup(const XNetStartupParams *params);
int WSAStartup(unsigned short versionRequested, WSAData *data);
