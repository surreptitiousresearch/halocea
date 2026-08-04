#pragma once
/* net_graph @ gGraph — on-screen network bandwidth graph (net_graph.c) */

#include "rectangle2d.h"
#include "dynamic_screen_vertex.h"
#include "net_graph_data_type.h"
#include "net_graph_data_direction.h"

typedef struct net_graph
{
    unsigned char            graph_empty;                   /* 0x0000 */
    unsigned char _pad0[3]; /* db-verified padding */
    unsigned int             period_start_ms;               /* 0x0004 */
    unsigned int             period_length_ms;              /* 0x0008 */
    net_graph_data_type      type;                          /* 0x000C */
    net_graph_data_direction direction;                     /* 0x0010 */
    int                      frame_height;                  /* 0x0014 */
    int                      frame_width;                   /* 0x0018 */
    float                    graph_height;                  /* 0x001C */
    float                    graph_width;                   /* 0x0020 */
    rectangle2d              extents;                       /* 0x0024 */
    rectangle2d              label_extents;                 /* 0x002C */
    rectangle2d              label_max_extents;             /* 0x0034 */
    rectangle2d              label_avg_extents;             /* 0x003C */
    dynamic_screen_vertex    border[5];                     /* 0x0044 */
    unsigned int             cumulative_bit_sent_total;     /* 0x00BC */
    unsigned int             cumulative_bit_received_total; /* 0x00C0 */
    unsigned int             cumulative_start_ms;           /* 0x00C4 */
    float                    cumulative_bps_sent;           /* 0x00C8 */
    float                    cumulative_bps_received;       /* 0x00CC */
    int                      current_sample_data;           /* 0x00D0 */
    int                      max_sample_lifetime;           /* 0x00D4 */
    int                      samples[320];                  /* 0x00D8 */
    dynamic_screen_vertex    vertices[320];                 /* 0x05D8 */
    int                      max;                           /* 0x23D8 */
    float                    average;                       /* 0x23DC */
    char                     label[512];                    /* 0x23E0 */
} net_graph;

extern net_graph gGraph;
extern unsigned int gNetGraphSamplePeriod;
