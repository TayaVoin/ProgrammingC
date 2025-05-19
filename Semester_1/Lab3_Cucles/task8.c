#include <stdio.h>

int main() {
	int s;
	int n;
	int i;
	printf("enter num: ");
	scanf("%d", &n);
	for (s=1,i=1;i<=n;i++){
		s = s*i;
	}
	printf("%d\n",s);
}