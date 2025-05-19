#include <stdio.h>
 
int main() {
	float a;
	char t;
	printf("Input temperature: ");
	scanf("%f%c", &a, &t);
	if (t=='c' || t=='C'){
		printf("%ff\n", a*1.8+32);
	}
	else if (t=='f' || t=='F'){
		printf("%fc\n", (a-32)*5/9);
	}
	else {
		printf("Ouput error\n");
	}
}