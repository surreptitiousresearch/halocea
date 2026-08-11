// cdtOFL::~cdtOFL @0x827E4D40 — free each owned array if non-null.
#include "../../headers/ws/cdt/cdtOFL.h"

extern "C" void dlFree(void *ptr);

cdtOFL::~cdtOFL()
{
    if (idList) dlFree(idList);
    if (nID2List) dlFree(nID2List);
    if (startID2List) dlFree(startID2List);
    if (id2List) dlFree(id2List);
}
