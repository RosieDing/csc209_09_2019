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
    
    if(nbytes % 8 != 0){
        nbytes = nbytes +(8 - (nbytes % 8));
    }
    //step1: check available empty block in free list
    int size = freelist->size;
    struct block *current = freelist;
    struct block *previous = NULL;
    struct block *next = freelist->next;
    while(next != NULL && size != nbytes){
        //printf("%s\n", "entering check freelist1");
        current = current->next;
        previous = current;
        next = current->next;
        size = current->size;
    }
    //printf("%s\n", "out check freelist1");

    //Case 1: find the empty fitting block available for new block
    if (current->size == nbytes){
       // printf("%s\n", "entering same length");
        //remove the block from freelist
        previous->next = next;
        //add it to the front of allocated_list
        current->next = allocated_list;
        allocated_list = current;
        return allocated_list->addr;
    }

    //Case 2: no empty block has the fitted size
    //step a: find the first bigger block
    current = freelist;
    previous = NULL;
    next = freelist->next;

    while(next != NULL && size < nbytes){
        // printf("%s\n", "entering check freelist2");
        current = current->next;
        previous = current;
        next = current->next;
        size = current->size;
    }

    //found the block which could be splited
    if(size > nbytes){
        //printf("%s\n", "entering create new");
        current->size = size - nbytes;
        //create a new block and add to the front of all_list
        struct block *new_block = malloc(sizeof(struct block));

        new_block->addr = current->addr;
        current->addr += nbytes;
        new_block->size = nbytes;
        new_block->next = allocated_list;
        allocated_list = new_block;
        //printf("smalloc    [addr: %p, size: %d]\n", allocated_list->addr, allocated_list->size);
       // printm_list(allocated_list);
        return allocated_list->addr;
    }

    return NULL;

}


int sfree(void *addr) {
	struct block *current = allocated_list;
    struct block *previous = NULL;
    struct block *next = allocated_list->next;
    if(addr == NULL){
        return -1;
    }

    while(next != NULL && current->addr != addr){
        previous = current;
        current = current->next;
        next = current->next;
    }
    if(next == NULL && current->addr!= addr){//cannot find the address
        return -1;
    }
    //step1: in this step remove the block from the allocated_list
    previous->next = next;

    //step2: add the block into the freelist (free list: increasing address)
    struct block *previous_free = NULL;
    struct block *current_free = freelist;
    struct block *next_free = freelist->next;

    while(next != NULL && addr > current->addr){
        previous_free = current_free;
        current_free = current_free->next;
        next_free = current_free->next;
    }
    if(next == NULL && addr > current->addr){//add at the end
        current_free->next = current;
        current->next = NULL;
        return 0;
    }
    //printf("%p\n", current_free->addr);
    //now addr is smaller than current address-> insert before current_free
    if(previous_free == NULL){//add at the first digit
        current->next = freelist;
        freelist = current;
        return 0;
    }

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
    //printf("%s%p\n", "init free", mem);  
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

