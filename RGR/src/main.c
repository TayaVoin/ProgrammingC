#include "common.h"
#include "stats.h"
#include "training.h"

void menu() {
    while (1) {
        printf("\n1. Тренировка\n2. Сложность\n3. Статистика\n4. Рекорды\n5. Выход\nВыберите: ");
        char c=getchar(); clear_buf();
        if (c=='1') start_training();
        else if (c=='2') select_diff();
        else if (c=='3') show_stats_menu();
        else if (c=='4') show_leaderboard();
        else if (c=='5') { save_stats(); save_leader(); break; }
        else printf("Неверно\n");
    }
}

int main() {
    srand(time(NULL));
    load_stats();
    load_leader();
    menu();
    return 0;
}