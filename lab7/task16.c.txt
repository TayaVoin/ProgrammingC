#include <stdio.h>
#include <string.h>

void main() {
    char s[256];
    fgets(s,256,stdin);

    int q = strlen(s);
    if (s[q-1]=='\n') s[q-1] ='\0';

    for (int i=q-1;i>= 0;i--) {
        if (s[i] == ' ' || i == 0) {
            int j=(i==0)?i:i+1;
            while (s[j] != ' ' && s[j] != '\0') {
                printf("%c", s[j]);
                j++;
            }
            if (i!=0) printf(" ");
        }
    }

    printf("\n");
}