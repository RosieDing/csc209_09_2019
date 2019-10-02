#include <stdio.h>
#include <stdlib.h>

void read_image( int num_rows, int num_cols, int arr[num_rows][num_cols], FILE *fp);
                
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]);

int count_cells(int num_rows, int num_cols,int arr[num_rows][num_cols]);
// Remember to include the function prototype for count_cells


int main(int argc, char **argv) {
    // Print a message to stderr and exit with an argument of 1 if there are
    // not the right number of parameters, or the second argument is not -p
    FILE *file;
    file = fopen("sample", "r");
    //file = fopen("A2.txt", "r");

    if (file == NULL)
    {
    	exit(1);
    }

    int num_rows, num_cols;

    fscanf(file, "%d", &num_rows);
    printf("%d\n", num_rows);
    fscanf(file, "%d", &num_cols);
    printf("%d\n", num_cols);

    int array[num_rows][num_cols];

    read_image(num_rows, num_cols, array, file);
    print_image(num_rows, num_cols, array);

    printf("%s\n","show digits");
    printf("%d\n", count_cells(num_rows, num_cols, array));
    fclose(file);
    

    return 0;

}