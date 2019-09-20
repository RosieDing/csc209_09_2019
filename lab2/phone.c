#include <stdio.h>
#include <stdlib.h>

int main(){
	char phone[11];
	int index;
	
	scanf("%s", phone);
	scanf("%d", &index);

	if(index >= 0 && index <= 9){
		printf("%c", phone[index]);
		return 0;
	}else if(index == -1){
		printf("%s", phone);
		return 0;
	}
	else{
		printf("ERROR");
		return 1;
	}
}