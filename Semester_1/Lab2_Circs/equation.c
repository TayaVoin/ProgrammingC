#include <stdio.h>
 
int main() {
	float x;
	float y;
	printf("enter num: ");
	scanf("%f%f", &x, &y);
	if (y<=x && x<=1 && y<=1){
		printf("YES\n");
	}
	else {
		printf("NO\n");
	}
}