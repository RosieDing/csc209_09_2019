#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 4096 * 64


/* In this tes */

int main(void) {

    mem_init(SIZE);
    
    char *ptrs[10];
    int i;

    /* Call smalloc 3 times */
    
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
    // ptrs[4] = smalloc(262072);
    // if (ptrs[4] == NULL)
    // {
    //     fprintf(stderr, "%s\n", "Cannot smalloc the blocks");
    // }else{
    // write_to_mem(262072,ptrs[4],4);
    // printf("%s\n%p\n", "Add new block: ",ptrs[4]);}

    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();

    

    printf("freeing %p result = %d\n", ptrs[0], sfree(ptrs[0]));
    printf("freeing %p result = %d\n", ptrs[1], sfree(ptrs[1]));
    printf("freeing %p result = %d\n", ptrs[2], sfree(ptrs[2]));
    printf("freeing %p result = %d\n", ptrs[3], sfree(ptrs[3]));
    //printf("freeing %p result = %d\n", ptrs[4], sfree(ptrs[4]));
    //printf("freeing %p result = %d\n", ptrs[1], sfree(ptrs[1]));

//     ptrs[4] = smalloc(262072);
//     if (ptrs[4] == NULL)
//     {
//         fprintf(stderr, "%s\n", "Cannot smalloc the blocks");
//     }else{
//     write_to_mem(262072,ptrs[4],4);
//     printf("%s\n%p\n", "Add new block: ",ptrs[4]);
// }

    ptrs[4] = smalloc(16);
    write_to_mem(16,ptrs[4],4);
    printf("%s\n%p\n", "Add new block: ",ptrs[4]);

    printf("List of free blocks:\n");
    print_free();

    printf("List of allocated blocks:\n");
    print_allocated();


    ptrs[5] = smalloc(262072);
    write_to_mem(262072,ptrs[5],5);
    printf("%s\n%p\n", "Add new block: ",ptrs[5]);

    printf("List of free blocks:\n");
    print_free();

    printf("List of allocated blocks:\n");
    print_allocated();

    mem_clean();
    return 0;
}
