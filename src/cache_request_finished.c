/* cache_request_finished @0x83753F60 */
#include "headers/cache_request.h"

void cache_request_finished(cache_request *finished_request)
{
    *finished_request->params.finished_flag = 1;
    finished_request->pending = 0;
    finished_request->running = 0;
}
