#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 4096 * 64


/* Simple test for smalloc and sfree. */

int main(void) {

    mem_init(SIZE);
    
    char *ptrs[10];
    int i;

    /* Call smalloc 4 times */
    
    for(i = 0; i < 3; i++) {
        int num_bytes = (i+1) * 10;
    
        ptrs[i] = smalloc(num_bytes);
        write_to_mem(num_bytes, ptrs[i], i);
    }
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();

    
    ptrs[3] = smalloc(262072);
    write_to_mem(262072,ptrs[3],3);
    printf("%s\n%p\n", "Add new block: ",ptrs[3]);

    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();

    // printf("freeing %p result = %d\n", ptrs[3], sfree(ptrs[3]));
     ptrs[4] = smalloc(262072);
     printf("%p\n", ptrs[4]);
    write_to_mem(262072,ptrs[4],4);
    printf("%s\n%p\n", "Add new block: ",ptrs[4]);

    // printf("freeing %p result = %d\n", ptrs[1], sfree(ptrs[1]));
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();

    // printf("freeing %p result = %d\n", ptrs[3], sfree(ptrs[3]));
    // printf("List of allocated blocks:\n");
    // print_allocated();
    // printf("List of free blocks:\n");
    // print_free();
    

    // ptrs[4] = smalloc(2);
    // write_to_mem(2,ptrs[4],4);
    // printf("%s\n%p\n", "add new block",ptrs[4]);
    // print_allocated();
    // printf("List of free blocks:\n");
    // print_free();


    mem_clean();
    return 0;
}
