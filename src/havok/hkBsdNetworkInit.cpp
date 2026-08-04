#include "../headers/havok/hkError.h"
#include "../headers/havok/hkBool.h"
#include "../headers/havok/xbox_net_boundary.h"

extern hkBool g_defaultPlatformInitOnce;

/* One-time platform network bring-up: start XNet with a minimal config (secure
   bypass, cfgFlags=1) then Winsock 2.2. On WSAStartup failure, route a message
   through the global hkError sink. Guarded by g_defaultPlatformInitOnce. */
void hkBsdNetworkInit(void)
{
    XNetStartupParams params;
    WSAData wsaData;

    if (g_defaultPlatformInitOnce.m_bool)
        return;

    /* zero every knob, then set struct size + the secure-socket bypass flag. */
    params.cfgSizeOfStruct = 13;
    params.cfgFlags = 1;
    params.cfgSockMaxDgramSockets = 0;
    params.cfgSockMaxStreamSockets = 0;
    params.cfgSockDefaultRecvBufsizeInK = 0;
    params.cfgSockDefaultSendBufsizeInK = 0;
    params.cfgKeyRegMax = 0;
    params.cfgSecRegMax = 0;
    params.cfgQosDataLimitDiv4 = 0;
    params.cfgQosProbeTimeoutInSeconds = 0;
    params.cfgQosProbeRetries = 0;
    params.cfgQosSrvMaxSimultaneousResponses = 0;
    params.cfgQosPairWaitTimeInSeconds = 0;
    XNetStartup(&params);

    if (WSAStartup(0x0202, &wsaData) == -1)
    {
        hkErrStream stream;
        char buffer[512];

        hkErrStream_construct(&stream, buffer, 512);
        hkOstream_insertString((hkOstream *)&stream, "(Windows)WSAStartup failed with error!");
        if (hkError_message(hkError_s_instance, MESSAGE_ERROR, 840443384, buffer,
                            ".\\System\\Io\\Socket\\Bsd\\hkBsdSocket.cpp", 72)) /* backslashes escaped 2026-07-30 */
        {
            __trap();
        }
        hkOstream_destruct((hkOstream *)&stream);
    }

    g_defaultPlatformInitOnce.m_bool = 1;
}
