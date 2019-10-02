#include <stdio.h>
    
/* Reads the image from the open file fp into the two-dimensional array arr 
 * num_rows and num_cols specify the dimensions of arr
 */
void read_image( int num_rows, int num_cols, int arr[num_rows][num_cols], FILE *fp) {
	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j ++){
			fscanf(fp, "%d", &(arr[i][j]));
			//printf("%d", arr[i][j]);
		}
	}
}

int find_repeat(int i, int j,int value, int num_rows, int num_cols,int arr[num_rows][num_cols]){
	if(arr[i][j-1] == value){
		return 0;
	}else if(arr[i+1][j] == value){
		return find_repeat(i+1, j,value,num_rows,num_cols,arr);
	}
	return 1;
}



int count_cells(int num_rows, int num_cols,int arr[num_rows][num_cols]){
	int sum = 0;
	int c;

	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j < num_cols; j++){
			if(arr[i][j] != 0){
				if(arr[i-1][j] != arr[i][j]){
					int value = arr[i][j];
					c = find_repeat(i, j,value,num_rows,num_cols,arr);
					if(c != 0){
						sum += 1;
					}
				}
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
		printf("%d", arr[i][j]);
	}
}
}

// int search_cell(int i, int j, int value, int num_rows, int num_cols, int arr[num_rows][num_cols]){
// 	if(i > 0){
// 	if(arr[i-1][j] == value){//search the value of upper pixes 
// 		return 0;
// 	}
// 	}
// 	if(j > 0){
// 		if (arr[i][j-1] == value)
// 		{
// 			return 0;
// 		}
// 	}
// 	if(arr[i][j+1] == value){
// 		return 1 + search_cell(i, j+1, value, num_rows, num_cols, arr);
// 	}else{
// 		return 0;
// 	}

// }

// /* TODO: Write the count_cells function */
// int count_cells(int num_rows, int num_cols,int arr[num_rows][num_cols]){
// int sum = 0;
// int count = 0;
// int value = 0;

// 	for(int i = 0; i < num_rows; i++){
// 		for(int j = 0; j< num_cols; j++){
// 			printf("%s\n", "start the pint");
// 			printf("%d\n", arr[i][j]);
// 			if(arr[i][j] != 0){
// 				value = arr[i][j];
// 				printf("%s\n", "entering block");


// 				count = search_cell(i, j, value, num_rows, num_cols, arr);

// 				if(i == 0){
// 					if(j == 0){
// 						if(count == 0){
// 							sum += 1;
// 							printf("%s", "count add");
// 							printf("%d\n", sum);
// 							printf("%s\n", "num_rows, num_cols");
// 							printf("%d%d\n",i,j);
// 						}
// 						else{
// 							sum += 1;
// 							j+= count;
// 						}
// 					}else{
// 						if(arr[i][j-1] != value){
// 							sum += 1;
// 							printf("%s", "count add");
// 							printf("%d\n", sum);
// 							printf("%s\n", "num_rows, num_cols");
// 							printf("%d%d\n",i,j);
// 						}
// 						j+=1;
// 					}
// 				}else if(j == 0){
// 					if(arr[i-1][j+count] != value){
// 						sum +=1;
// 						printf("%s", "count add");
// 							printf("%d\n", sum);
// 							printf("%s\n", "num_rows, num_cols");
// 							printf("%d%d\n",i,j);
// 					}
// 					j+=1;
// 				}
// 				else{
// 					if(arr[i-1][j+count] != value && arr[i][j-1] != value){
// 							sum += 1;
// 							printf("%s", "count add");
// 							printf("%d\n", sum);
// 							printf("%s\n", "num_rows, num_cols");
// 							printf("%d%d\n",i,j);
// 						}
// 						j += count;
// 					}


// 			}

// 			printf("%s\n", "at the end print the index");
// 			printf("%d\n", i);
// 			printf("%d\n", j);
// 		}
// 	}
// 	printf("%s\n", "end of iteration:");
// 	printf("%d\n", sum);
// 	return sum;

// }
