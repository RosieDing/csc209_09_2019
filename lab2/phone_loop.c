#include <stdio.h>
#include <stdlib.h>

int main(){
	char phone[11];
	scanf("%s", phone);
	int index = 0;
	int error_ms = 0;
	int retval = 0;
	while((retval=scanf("%d", &index))!= EOF){
		if(index >= 0 && index <= 9){
			printf("%c\n", phone[index]);
		}else if(index == -1){
			printf("%s\n", phone);
		}
		else{
			printf("ERROR\n");
			error_ms = 1;
		}
	}

	return error_ms;
	
}