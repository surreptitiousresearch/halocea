#pragma once
/* widget_type_definition — vtable/descriptor for an object widget type (antenna, fluid, mesh,
 * contrail, light-volume). widget_type_definitions[5] indexes these by widget type code. Layout
 * from the database. Render argument types are left opaque (forward-declared). */

struct render_lighting;
struct render_animation;

typedef struct widget_type_definition
{
    unsigned int    group_tag;              /* 0x00 */
    unsigned __int8 needs_lighting;         /* 0x04 */
    char            _pad05[3];              /* 0x05 */
    void          (*initialize)(void);              /* 0x08 */
    void          (*initialize_for_new_map)(void);  /* 0x0C */
    void          (*dispose_from_old_map)(void);    /* 0x10 */
    void          (*dispose)(void);                 /* 0x14 */
    int           (*new_proc)(int);                 /* 0x18 */
    void          (*delete_proc)(int);              /* 0x1C */
    void          (*update)(float);                 /* 0x20 */
    void          (*render_proc)(int, int, const struct render_lighting *, const struct render_animation *); /* 0x24 */
} widget_type_definition;

extern widget_type_definition widget_type_definitions[5];
