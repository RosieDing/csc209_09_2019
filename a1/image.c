#include <stdio.h>
    
/* Reads the image from the open file fp into the two-dimensional array arr 
 * num_rows and num_cols specify the dimensions of arr
 */
void read_image( int num_rows, int num_cols, int arr[num_rows][num_cols], FILE *fp) {
	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j ++){
			fscanf(fp, "%d", &(arr[i][j]));
		}
	}
}

void find_repeat(int i, int j,int value, int num_rows, int num_cols,int arr[num_rows][num_cols]){
	arr[i][j] = 0;
	if(arr[i][j-1] == value)
	{
		find_repeat(i,j-1, value,num_rows,num_cols,arr);
	}
	if(arr[i-1][j] == value)
	{
		find_repeat(i-1, j, value,num_rows,num_cols,arr);
	}
	if(arr[i+1][j] == value)
	{
		find_repeat(i+1, j, value,num_rows, num_cols,arr);
	}
	if(arr[i][j+1] == value)
	{
		find_repeat(i, j+1, value,num_rows,num_cols,arr);
	}
}



int count_cells(int num_rows, int num_cols,int arr[num_rows][num_cols]){
	int sum = 0;

	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			if(arr[i][j] != 0){
				find_repeat(i,j,arr[i][j], num_rows,num_cols,arr);
				sum += 1;
			}
		}
	}
	return sum;

}



/* Print to standard output the contents of the array arr */
void print_image(int num_rows, int num_cols, int arr[num_rows][num_cols]) {
int i, j;
for(i = 0; i < num_rows; i++){
	for(j = 0; j < num_cols; j++){
		printf(" %d", arr[i][j]);
	}
	printf("\n");
}
}


