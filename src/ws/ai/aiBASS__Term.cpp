// aiBASS::Term @ 0x83240500
// ?Term@aiBASS@@MAAXXZ
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"

extern sslSYSTEM *gsSslSystem; // ?gsSslSystem@@3PAVsslSYSTEM@@A — global SSL system instance

// Tear down: if a scripted-sound object was created, remove it from the SSL system.
void aiBASS::Term()
{
    if (sslObj.pObject)
    {
        sslOBJ_REF ref(sslObj);
        gsSslSystem->RemoveObject(ref);
    }
}
