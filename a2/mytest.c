#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 4096 * 64


/* In this interesting test I design an interesting condition: 
1. I tried to add 3 blocks from freelist;
2. I tried to smalloc all the memory left in free to allocated_list;
3. I tried to sfree all allocated_list back to freelist;
4. I tried to smalloc the last two block back to allocated_list;

In this test, I tried to test all the boundary point of the list:
which means that: smalloc when allocated_list is empty; 
                  sfree when the freelist is empty or nearly empty;
                  smalloc to make freelist empty;
                  sfree to make allocated_list is empty;

Why Interesting:
since it is easy to ignore the checking of the boundary before each operation, when we write this program.

*/

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


    printf("freeing %p result = %d\n", ptrs[0], sfree(ptrs[0]));
    printf("freeing %p result = %d\n", ptrs[1], sfree(ptrs[1]));
    printf("freeing %p result = %d\n", ptrs[2], sfree(ptrs[2]));
    printf("freeing %p result = %d\n", ptrs[3], sfree(ptrs[3]));
    

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
