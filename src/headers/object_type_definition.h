#pragma once
/* object_type_definition — the per-object-type vtable/descriptor (biped, vehicle, weapon, equipment,
 * projectile, scenery, machine, control, ...). object_type_definitions[12] indexes these by type code;
 * each may chain a list of part_definitions[16] (super-type + sub-types) whose datum_* callbacks the
 * object system fans out to. Layout from the database. Referenced argument types are left opaque
 * (forward-declared) — only the table shape matters to the object update/delete fan-out. */

struct object_placement_data;
struct real_orientation;
struct real_matrix4x3;
struct message_delta_processor_header;
struct network_game_client;
typedef int message_delta_processor_message_definition_type;
#include <stdint.h>
#include "message_delta_processor_mode.h" // canonical enum typedef (was a conflicting `typedef int`)

typedef struct object_type_definition
{
    const char     *name;                                   /* 0x00 */
    unsigned int    group_tag;                              /* 0x04 */
    int16_t         game_datum_size;                        /* 0x08 */
    int16_t         placement_tag_block_offset;             /* 0x0A */
    int16_t         palette_tag_block_offset;               /* 0x0C */
    int16_t         placement_tag_block_element_size;       /* 0x0E */
    const message_delta_processor_message_definition_type update_message_type; /* 0x10 */
    /* attested void(void): bctrl @0x83701638 sets up no args (r3-r6 = loop residue); all
     * registered targets (bipeds_initialize et al.) are void(void) */
    void          (*initialize)(void);                       /* 0x14 */
    void          (*dispose)(void);                         /* 0x18 */
    void          (*initialize_for_new_map)(void);          /* 0x1C */
    void          (*dispose_from_old_map)(void);            /* 0x20 */
    void          (*datum_adjust_placement)(int, struct object_placement_data *); /* 0x24 */
    uint8_t (*datum_new)(int);                      /* 0x28 */
    void          (*datum_place)(int, void *);              /* 0x2C */
    void          (*datum_delete)(int);                     /* 0x30 */
    uint8_t (*datum_update)(int);                   /* 0x34 */
    void          (*datum_export_function_values)(int);     /* 0x38 */
    void          (*handle_deleted_object)(int, int);       /* 0x3C */
    void          (*handle_region_destroyed)(int, int16_t, unsigned int); /* 0x40 */
    uint8_t (*handle_parent_destroyed)(int);        /* 0x44 */
    void          (*datum_preprocess_node_orientations)(int, struct real_orientation *); /* 0x48 */
    void          (*datum_postprocess_node_matrices)(int, struct real_matrix4x3 *); /* 0x4C */
    void          (*reset)(int);                            /* 0x50 */
    void          (*disconnect_from_structure_bsp)(int);    /* 0x54 */
    void          (*notify_impulse_sound)(int, int, int);   /* 0x58 */
    void          (*render_debug)(int);                     /* 0x5C */
    uint8_t (*should_render_debug_object_network_visualization)(int); /* 0x60 */
    int           (*object_new_to_network)(int, void *, int); /* 0x64 */
    void          (*update_baseline)(int);                  /* 0x68 */
    int           (*build_update_delta)(int, void *, int, const message_delta_processor_mode); /* 0x6C */
    void          (*process_update_delta)(int, struct message_delta_processor_header *, struct network_game_client *); /* 0x70 */
    uint8_t (*should_try_sending_incremental)(int); /* 0x74 */
    uint8_t (*should_send_incremental_to_player)(int, int); /* 0x78 */
    void          (*incremental_sent)(int);                 /* 0x7C */
    struct object_type_definition *part_definitions[16];    /* 0x80 — null-terminated part chain */
    struct object_type_definition *next;                    /* 0xC0 */
} object_type_definition;

extern object_type_definition *object_type_definitions[12];
