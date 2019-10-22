#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"



void *mem;
struct block *freelist;
struct block *allocated_list;

void *smalloc(unsigned int nbytes) {

    if(nbytes == 0){
        return NULL;
    }
    
    if(nbytes % 8 != 0){
        nbytes = nbytes +(8 - (nbytes % 8));
    }
    //------check available empty block in free list
    //step 1 : find the first bigger/equal block
    int size = freelist->size;
    struct block *current = freelist;
    struct block *previous = NULL;
    struct block *next = freelist->next;


    while(next != NULL && size < nbytes){
        current = current->next;
        previous = current;
        next = current->next;
        size = current->size;
    }

    //Case 1: find the empty fitting block size equal to new block
    if (size == nbytes){
        //remove the block from freelist
        if(previous == NULL){
            freelist = next;
        }else{
        previous->next = next;}

        //add it to the front of allocated_list
        current->next = allocated_list;
        allocated_list = current;
        printf("%p\n", freelist);

        return allocated_list->addr;
    }

    //Case 2: found the block which could be splited(first bigger block)
    if(size > nbytes){
        current->size = size - nbytes;
        //create a new block and add to the front of all_list
        //under this condition, we don't to reallocate the block 
        struct block *new_block = malloc(sizeof(struct block));

        new_block->addr = current->addr;
        current->addr += nbytes;
        new_block->size = nbytes;
        new_block->next = allocated_list;
        allocated_list = new_block;
        return allocated_list->addr;
    }

    return NULL;

}


int sfree(void *addr) {
    if(addr == NULL){//the given address is empty
        return -1;
    }
	struct block *current = allocated_list;
    struct block *previous = NULL;
    struct block *next = allocated_list->next;

    //find the block from allocated list
    while(next != NULL && current->addr != addr){
        previous = current;
        current = current->next;
        next = current->next;
    }
    if(next == NULL && current->addr!= addr){//cannot find the address
        return -1;
    }

    //step1: in this step remove the block from the allocated_list
    if(previous == NULL){//remove the block from the end of the allocated list
        allocated_list = next;
    }else{
    previous->next = next;
    }

    //step2: add the block into the freelist (free list: increasing address)
    struct block *previous_free = NULL;
    struct block *current_free = freelist;
    struct block *next_free = freelist->next;


    while(next_free != NULL && addr > current_free->addr){
        previous_free = current_free;
        current_free = current_free->next;
        next_free = current_free->next;
    }
   
   //the addr is the biggest in the free list
    if(next_free == NULL && addr > current_free->addr){
        current_free->next = current;
        current->next = NULL;
        return 0;
    }

    //now addr is smaller than current address-> insert before current_free
    if(previous_free == NULL){//add at the first digit
        current->next = freelist;
        freelist = current;
        return 0;
    }

    //insert in the middle
    previous_free->next = current;
    current->next = current_free;
    return 0;
}


/* Initialize the memory space used by smalloc,
 * freelist, and allocated_list
 * Note:  mmap is a system call that has a wide variety of uses.  In our
 * case we are using it to allocate a large region of memory. 
 * - mmap returns a pointer to the allocated memory
 * Arguments:
 * - NULL: a suggestion for where to place the memory. We will let the 
 *         system decide where to place the memory.
 * - PROT_READ | PROT_WRITE: we will use the memory for both reading
 *         and writing.
 * - MAP_PRIVATE | MAP_ANON: the memory is just for this process, and 
 *         is not associated with a file.
 * - -1: because this memory is not associated with a file, the file 
 *         descriptor argument is set to -1
 * - 0: only used if the address space is associated with a file.
 */
void mem_init(int size) {
    mem = mmap(NULL, size,  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if(mem == MAP_FAILED) {
         perror("mmap");
         exit(1);
    }

	allocated_list = NULL;
    freelist = malloc(sizeof(struct block));
    freelist->addr = mem;
    freelist->size = size;
    freelist->next = NULL; 
}


void mem_clean(){
    struct block *next = allocated_list->next;
    
    while(allocated_list != NULL){
        next = allocated_list->next;
        free(allocated_list);
        allocated_list = next;
    }

   
    next = freelist->next;
    
    while(freelist != NULL){
        next = freelist->next;
        free(freelist);
        freelist = next;
    }



}

