#include <stdio.h>

void one_print()
{
	int i=0;
	while(i < 100){
		printf("%d ", i);
		i++;
	}

	printf("\n");
}

void two_print(int i)
{

	if(i){
		two_print(i-1);
		printf("%d ", i-1);
	}else{
		printf("\n");
	}
}

int main()
{
	printf("%d\n", 2*2+1);
}
