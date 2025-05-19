#include <stdio.h>

int main() {
	int a;
	int s;
	int n;
	printf("enter num: ");
	scanf("%d", &a);
	for (int s=0,n=2;n<a;n++){
		if (a%n==0){
			s=1;
			printf("%d\n",n);
			break;
		}
	}
}