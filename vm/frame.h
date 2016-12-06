#ifndef VM_FRAME_H
#define VM_FRAME_H

/* Nathan and Matt's Implementation of the Frame table */
/* frame.h */
#include <list.h>

struct list frame_table;

struct frame_table_entry {
    struct list_elem elem;
    struct s_page_table_entry *spte;  /* pointer to entry in supplemental page table */
    void *frame; 
};

void init_frame_table(void);
void *frame_allocate(struct s_page_table_entry *spte);
void free_frame(void *frame);

#endif
