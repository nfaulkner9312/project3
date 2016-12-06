/* Nathan and Matt's Supplemental Page Table Implementation */
/* page.c */

#include "vm/page.h"
#include "userprog/process.h"
#include "vm/frame.h"
#include "threads/vaddr.h"

bool load_page(struct s_page_table_entry *spte) {
    if (spte->is_swap) {
        return load_from_swap(spte);
    } else {
        return load_from_file(spte);
    }
}

bool load_from_file(struct s_page_table_entry *spte) {

    /* Get a page of memory */
    uint8_t *frame = frame_allocate(spte);
    /* if frame_allocate return NULL return false */
    if (!frame) {
        return false;
    }
    
    /* Load this page */
    if (file_read_at(spte->file, frame, spte->read_bytes, spte->offset) != (int) spte->read_bytes) {
        free_frame(frame);
        return false;
    }
    memset(frame + spte->read_bytes, 0, spte->zero_bytes);
    
    /* Add the page to the process's address space. */
    if (!install_page(spte->user_va, frame, spte->writable)) {
        free_frame(frame);
        return false;
    }
    spte->is_resident = true;

    return true;
}

bool load_from_swap(struct s_page_table_entry *spte) {
    return true;
}

bool grow_stack(void *user_va) {
    struct s_page_table_entry *spte = malloc(sizeof(struct s_page_table_entry));
    
    // set fields of page table entry
    spte->user_va = pg_round_down(user_va);
    spte->writable = true;
    spte->is_swap = true;
    spte->is_mmap = false;
    
    /* Get a age of memory */
    uint8_t *frame = frame_allocate(spte);
    
    /* Add the page to the process's address space. */
    if (!install_page(spte->user_va, frame, spte->writable)) {
        free(spte);
        free_frame(frame);
        return false;
    }
    spte->is_resident = true;

    /* add to list */
    list_push_back(&thread_current()->spt_list, &spte->elem);

    return true;
}

struct s_page_table_entry* user_va2spte(void *user_va) {
    
    struct thread *cur=thread_current(); /*current parent thread*/
    struct list_elem *SPTLE; /*supplemental page table list element*/
    struct s_page_table_entry *spte;  /*pointer to supplemental page table entry*/

    for(SPTLE = list_begin(&(cur->spt_list)); SPTLE != list_end(&(cur->spt_list)); SPTLE=list_next(SPTLE)) {
        spte = list_entry(SPTLE, struct s_page_table_entry, elem); 

        if(spte->user_va == user_va){
            return spte; 
        }
    }

    /* if there was not spte with user virtual address equal to user_va */
    return NULL; 
}
