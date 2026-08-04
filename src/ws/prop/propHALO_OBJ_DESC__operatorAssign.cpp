#include "../../headers/ws/prop/propHALO_OBJ_DESC.h"

// propHALO_OBJ_DESC::operator=(const propHALO_OBJ_DESC&) @ 0x823E4570
// Memberwise copy-assign: propBASE_DESC base first, then every derived field.
propHALO_OBJ_DESC &propHALO_OBJ_DESC::operator=(const propHALO_OBJ_DESC &that)
{
    propBASE_DESC::operator=(that);
    this->isEntSslClassInited.val = that.isEntSslClassInited.val;
    this->sslEventId_OnChangeInA = that.sslEventId_OnChangeInA;
    this->sslEventId_OnChangeInB = that.sslEventId_OnChangeInB;
    this->sslEventId_OnChangeInC = that.sslEventId_OnChangeInC;
    this->sslEventId_OnChangeInD = that.sslEventId_OnChangeInD;
    this->sslEventId_OnChangeOutA = that.sslEventId_OnChangeOutA;
    this->sslEventId_OnChangeOutB = that.sslEventId_OnChangeOutB;
    this->sslEventId_OnChangeOutC = that.sslEventId_OnChangeOutC;
    this->sslEventId_OnChangeOutD = that.sslEventId_OnChangeOutD;
    this->objUse.state = that.objUse.state;
    this->timeTerm = that.timeTerm;
    return *this;
}
