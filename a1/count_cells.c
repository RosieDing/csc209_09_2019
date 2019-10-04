#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_image( int num_rows, int num_cols, int arr[num_rows][num_cols], FILE *fp);
                
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]);

int count_cells(int num_rows, int num_cols,int arr[num_rows][num_cols]);
// Remember to include the function prototype for count_cells


int main(int argc, char **argv) {
    // Print a message to stderr and exit with an argument of 1 if there are
    // not the right number of parameters, or the second argument is not -p
    if(argc < 2 || argc > 3){
        fprintf(stderr, "Usage: count_cells <imagefile.txt> [-p]");
        exit(1);
    }

    if(argc == 3){
        char *ms = argv[2];
        if(strcmp(ms,"-p") != 0){
        fprintf(stderr, "Usage: count_cells <imagefile.txt> [-p]");
        exit(1);
    }
    }
    char *file_name = argv[1];
    FILE *file;

    //file = fopen("sample", "r");
    file = fopen(file_name, "r");

    if (file == NULL)
    {
    	exit(1);
    }

    int num_rows, num_cols;

    fscanf(file, "%d", &num_rows);
    
    fscanf(file, "%d", &num_cols);

    int array[num_rows][num_cols];

    read_image(num_rows, num_cols, array, file);

    printf("Number of Cells is %d\n", count_cells(num_rows, num_cols, array));

    if(argc == 3){
        char *ms = argv[2];
        if(strcmp(ms,"-p") == 0){
        print_image(num_rows, num_cols, array);}
    }

    fclose(file);
    

    return 0;

}