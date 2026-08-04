#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiPLANNER::cbSetHOMaxAngleCorner @ 0x83253FC0
// ?cbSetHOMaxAngleCorner@aiPLANNER@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: unpack the float in argv[0] and forward it to implSetHOMaxAngleCorner.
// `caller` releases on return.
void aiPLANNER::cbSetHOMaxAngleCorner(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/,
                                      sslOBJ_REF /*caller*/)
{
    float angle = 0.0f;
    argv[0].GetValue<float>(angle, DSD_CONV_RETRIEVE);
    implSetHOMaxAngleCorner(angle);
    // caller destructor self-releases at scope end.
}
