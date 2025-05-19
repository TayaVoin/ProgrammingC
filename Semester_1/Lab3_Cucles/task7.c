#include <stdio.h>

int main() {
	int a;
	int n=2;
	int flag=0;
	printf("enter num: ");
	scanf("%d", &a);
	while (a%n!=0){
		n++;
		if (n==a){
			flag=1;
			printf("%d\n",n);
			break;
		}
	}
}