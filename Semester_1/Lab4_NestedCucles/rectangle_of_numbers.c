
#include <stdio.h>

int main() {
	int a;
	int num=1;
	printf("enter num: ");
	scanf("%d", &a);
	for (int i=0;i<a;i++){
		printf("\n");
			for (int j=0;j<a;j++){
				printf("%d",num);
				num++;
			}
	}
}
