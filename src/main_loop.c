/* main_loop @ 0x8368C330 — standalone Blam loop (used in editor/tool builds) */

extern void main_loop_init1(void);
extern void main_loop_init2(void);
extern int main_loop_update(void);
extern void main_loop_term(void); /* attested void */

void main_loop(void) /* attested void: tail r3 from void main_loop_term, 0 direct callers */
{
    main_loop_init1();
    main_loop_init2(); /* attested void(void): r3-thread removed */
    while ( main_loop_update() )
        ;
    main_loop_term();
}
