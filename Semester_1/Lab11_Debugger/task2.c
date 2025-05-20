#include <stdio.h>

typedef struct{
    char str[5]; //в размере учитываем дополнительный символ для окончания строки
    int num;
} NumberRepr;
void format(NumberRepr* number){
    sprintf(number->str, "%3d", number->num);
}
int main(){
    NumberRepr number = { .num = 1025 };
    format(&number);
    printf("str: %s\n", number.str);
    printf("num: %d\n", number.num);
    return 0;
}