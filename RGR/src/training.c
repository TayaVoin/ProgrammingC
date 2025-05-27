#include "common.h"
#include "training.h"

const char *texts_easy[] = {
    "Кот сидит на ковре.",
    "Солнце светит ярко.",
    "Я люблю программировать.",
    "Чай горячий и вкусный.",
    "Книга лежит на столе."
};
const char *texts_medium[] = {
    "Быстрая коричневая лиса прыгает через ленивую собаку.",
    "Сложные задачи требуют внимательного подхода и терпения.",
    "Программирование - это искусство создания полезных вещей из ничего.",
    "Клавиатурный тренажер помогает улучшить скорость печати.",
    "Практика ведет к совершенству, особенно в наборе текста."
};
const char *texts_hard[] = {
    "Философия - это систематическое изучение фундаментальных вопросов бытия, познания, ценностей и языка.",
    "Квантовая механика описывает поведение материи и энергии на атомном и субатомном уровнях.",
    "Алгоритмы и структуры данных являются основой компьютерных наук и программирования.",
    "Нейронные сети имитируют работу человеческого мозга для решения сложных задач машинного обучения.",
    "Теория относительности Эйнштейна изменила наше понимание пространства, времени и гравитации."
};
const char *texts_asdf[] = { "asdf asdf asdf asdf asdf asdf asdf asdf asdf asdf" };
const char *texts_jkl[] = { "jkl; jkl; jkl; jkl; jkl; jkl; jkl; jkl; jkl; jkl;" };
const char *texts_minecraft[] = {
    "/tp a s", "/setblock ~ ~ ~ water", "/tp ~ 100 ~",
    "/gamemode c", "/gamemode su", "/gamemode sp",
    "/gamemode a c", "/gamemode a su", "/gamemode a sp",
    "/give torch 64", "/give beef 64", "/enchant efficiency 5", "/enchant unbreaking 3"
};

char **texts_custom = NULL;
size_t custom_count = 0;
PlayerStats *players = NULL;
size_t player_count = 0;
LeaderEntry leaderboard[MAX_ENTRIES];
size_t leader_count = 0;
Diff current_diff = MEDIUM;

void current_timestamp(char *buf, size_t sz) {
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    strftime(buf, sz, "%Y-%m-%d %H:%M:%S", lt);
}

void clear_buf() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void show_results(int total, int correct, int wrong, double elapsed) {
    printf("\n=== Результаты ===\n");
    printf("Всего символов: %d\n", total);
    printf("Правильных: %d, Ошибок: %d\n", correct, wrong);
    printf("Время: %.2f🍺 \n", elapsed); 
    double speed = correct / (elapsed/60.0);
    double accuracy = (total>0? correct*100.0/total: 0);
    printf("Скорость: %.1f зн/мин, Точность: %.1f%%\n", speed, accuracy);
    
    char nick[MAX_NICK_LEN];
    printf("Введите ник (Enter чтобы пропустить): ");
    if (!fgets(nick, MAX_NICK_LEN, stdin)) return;
    nick[strcspn(nick, "\r\n")] = '\0';
    if (strlen(nick)==0) return;

    PlayerStats *p = NULL;
    for (size_t i=0; i<player_count; i++) {
        if (strcmp(players[i].nickname, nick)==0) { p=&players[i]; break; }
    }
    if (!p) {
        players = realloc(players, (player_count+1)*sizeof(PlayerStats));
        p = &players[player_count];
        strncpy(p->nickname, nick, MAX_NICK_LEN);
        p->stats = NULL;
        p->count = 0;
        player_count++;
    }
    p->stats = realloc(p->stats, (p->count+1)*sizeof(Stat));
    Stat *s = &p->stats[p->count++];
    current_timestamp(s->date, sizeof(s->date));
    s->speed = speed;
    s->accuracy = accuracy;
    s->elapsed = elapsed;
    s->correct = correct;
    s->wrong = wrong;
    s->difficulty = current_diff;

    int found = 0;
    for (size_t i=0; i<leader_count; i++) {
        if (strcmp(leaderboard[i].nickname, nick)==0) {
            if (speed > leaderboard[i].speed) {
                leaderboard[i].speed = speed;
                current_timestamp(leaderboard[i].date, sizeof(leaderboard[i].date));
            }
            found = 1; break;
        }
    }
    if (!found && leader_count < MAX_ENTRIES) {
        LeaderEntry *e = &leaderboard[leader_count++];
        strncpy(e->nickname, nick, MAX_NICK_LEN);
        e->speed = speed;
        current_timestamp(e->date, sizeof(e->date));
    }
    for (size_t i=0; i<leader_count; i++) {
        for (size_t j=i+1; j<leader_count; j++) {
            if (leaderboard[j].speed > leaderboard[i].speed) {
                LeaderEntry tmp = leaderboard[i]; leaderboard[i] = leaderboard[j]; leaderboard[j] = tmp;
            }
        }
    }
    if (leader_count>10) leader_count = 10; //вывод первых 10 лидеров
}

void start_training() {
    const char *text;
    switch (current_diff) {
        case EASY: text = texts_easy[rand()%5]; break;
        case MEDIUM: text = texts_medium[rand()%5]; break;
        case HARD: text = texts_hard[rand()%5]; break;
        case ASDF: text = texts_asdf[0]; break;
        case JKL: text = texts_jkl[0]; break;
        case MINECRAFT: text = texts_minecraft[rand()%13]; break;
        case CUSTOM: text = texts_custom[rand()%custom_count]; break;
    }
    
    printf("\nТренировка (%d): %s\n", current_diff, text);
    printf("Начинайте ввод: \n");
    
    // Засекаем время сразу после вывода текста
    clock_t t0 = clock();
    
    char input[MAX_TEXT_LEN];
    if (!fgets(input, MAX_TEXT_LEN, stdin)) return;
    
    clock_t t1 = clock();
    double elapsed = (t1 - t0) / (double)CLOCKS_PER_SEC;
    
    input[strcspn(input, "\r\n")] = '\0';
    int len = strlen(text), ulen = strlen(input);
    int correct=0, wrong=0;
    int m = len>ulen? len: ulen;
    for (int i=0; i<m; i++) {
        if (i<len && i<ulen && text[i]==input[i]) correct++; else wrong++;
    }
    show_results(correct+wrong, correct, wrong, elapsed*100000);
}

void select_diff() {
    printf("\n=== difficulty choice ===\n");
    printf("1. EASY\n" );
    printf("2. MEDIUM\n" );
    printf("3. HARD\n" );
    printf("4. ASDF\n" );
    printf("5. JKL;\n" );
    printf("6. Your text\n" );
    printf("\n");
    char c = getchar(); clear_buf();
    if (c=='1') current_diff=EASY;
    else if (c=='2') current_diff=MEDIUM;
    else if (c=='3') current_diff=HARD;
    else if (c=='4') current_diff=ASDF;
    else if (c=='5') current_diff=JKL;
    else if (c=='6') {
        char tmp[MAX_TEXT_LEN];
        printf("Input text: "); fgets(tmp, MAX_TEXT_LEN, stdin);
        tmp[strcspn(tmp,"\r\n")]='\0';
        if (custom_count>0) free(texts_custom);
        custom_count=1;
        texts_custom = malloc(sizeof(char*));
        texts_custom[0] = strdup(tmp);
        current_diff=CUSTOM;
    } else if (c=='m') { current_diff=MINECRAFT; }
    else { printf("Неверный выбор\n"); }
    printf("Выбран уровень %d\n", current_diff);
}