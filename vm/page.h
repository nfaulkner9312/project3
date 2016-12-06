#ifndef VM_PAGE_H
#define VM_PAGE_H

/* Nathan and Matt's Supplemental Page Table Implementation */
/* page.h */


#include <list.h>

struct s_page_table_entry {
    bool is_swap;
    bool is_mmap;
    bool writable;
    bool is_resident;
     
    void *user_va;

    /* when the spte is a file */
    struct file *file;
    size_t offset;
    size_t read_bytes;
    size_t zero_bytes;

    /* when the spte is in the swap */
    /* index or pointer? (probably depends on how the swap file is done) */

    struct list_elem elem;
};

bool load_page(struct s_page_table_entry *spte);
/* this will determine if the s_page_table_entry is a file or swap and will then load accordingly */
/* also if the page is already loaded in physical memory (determined by is_resident) */
/* then you don't have to do anything here */

bool load_from_file(struct s_page_table_entry *spte);
bool load_from_swap(struct s_page_table_entry *spte);
/* these functions will: */
/* 1) do the allocating of frames (alloc_frame declared in frame.h) */
/* 2) memset */
/* 3) install_page */
/* 4) the page table entry is now in physical memory so set is_resident to true */
/* this is basically replacing what we commented out in load_segment in process.c */


/* grow the stack as needed */
bool grow_stack(void *user_va);

struct s_page_table_entry* user_va2spte(void *user_va);
#endif

