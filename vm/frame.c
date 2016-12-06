/* The frame table implementation */

#include "vm/frame.h"
#include "filesys/file.h"
#include "userprog/pagedir.h"
#include "threads/malloc.h"
#include "threads/palloc.h"

void init_frame_table(void) {
    list_init(&frame_table);
}

void* frame_allocate( struct s_page_table_entry *spte) {

    void *frame = palloc_get_page(PAL_USER);
    if (frame) {
        /* add frame to frame table */
        struct frame_table_entry *fte = malloc(sizeof(struct frame_table_entry));
        fte->frame = frame;
        fte->spte = spte;
        list_push_back(&frame_table, &fte->elem);
    } else {
        /* evict frame to make room */
        /* if frame could not be evicted than swap is full, PANICCC!!! */
        
        /* frame = evict_frame(); */
    }
    return frame;
}

void free_frame(void *frame) {

    /* free up this frame */
}

void* evict_frame(void) {
    /* this is probably fairly involved, check dirty bits and such */
    /* pagedir_is_dirty */
    /* pagedir_is_accessed */
    return palloc_get_page(PAL_USER);
}

