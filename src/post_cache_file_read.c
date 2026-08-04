#include "headers/cache_request.h"

void post_cache_file_read(cache_request *finished_request)
{
    *finished_request->params.finished_flag = 1;
    finished_request->pending = 0;
    finished_request->running = 0;
}
