#pragma once

typedef enum net_graph_data_direction /* : __int32 — enum-base is C++; underlying width 4 preserved (values fit int) */
{
    net_graph_data_direction_sent       = 0,
    net_graph_data_direction_received   = 1,
    NUMBER_OF_NET_GRAPH_DATA_DIRECTIONS = 2,
    net_graph_data_direction_invalid    = -1,
} net_graph_data_direction;
