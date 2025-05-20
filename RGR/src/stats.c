#include "common.h"
#include "stats.h"

void load_stats() {
    json_error_t error;
    json_t *root = json_load_file(STATS_FILE, 0, &error);
    
    if (!root) return;
    if (!json_is_object(root)) {
        json_decref(root);
        return;
    }
    
    const char *nickname;
    json_t *stats_array;
    
    json_object_foreach(root, nickname, stats_array) {
        if (!json_is_array(stats_array)) continue;
        
        players = realloc(players, (player_count + 1) * sizeof(PlayerStats));
        PlayerStats *p = &players[player_count];
        
        strncpy(p->nickname, nickname, MAX_NICK_LEN);
        p->count = json_array_size(stats_array);
        p->stats = malloc(p->count * sizeof(Stat));
        
        for (size_t i = 0; i < p->count; i++) {
            json_t *stat_obj = json_array_get(stats_array, i);
            Stat *s = &p->stats[i];
            
            strncpy(s->date, json_string_value(json_object_get(stat_obj, "date")), sizeof(s->date));
            s->speed = json_number_value(json_object_get(stat_obj, "speed"));
            s->accuracy = json_number_value(json_object_get(stat_obj, "accuracy"));
            s->elapsed = json_number_value(json_object_get(stat_obj, "time"));
            s->correct = json_integer_value(json_object_get(stat_obj, "correct"));
            s->wrong = json_integer_value(json_object_get(stat_obj, "wrong"));
            s->difficulty = json_integer_value(json_object_get(stat_obj, "difficulty"));
        }
        
        player_count++;
    }
    
    json_decref(root);
}

void load_leader() {
    json_error_t error;
    json_t *root = json_load_file(LEADER_FILE, 0, &error);
    
    if (!root) return;
    if (!json_is_array(root)) {
        json_decref(root);
        return;
    }
    
    leader_count = json_array_size(root);
    if (leader_count > MAX_ENTRIES) leader_count = MAX_ENTRIES;
    
    for (size_t i = 0; i < leader_count; i++) {
        json_t *entry = json_array_get(root, i);
        LeaderEntry *e = &leaderboard[i];
        
        strncpy(e->nickname, json_string_value(json_object_get(entry, "nickname")), MAX_NICK_LEN);
        e->speed = json_number_value(json_object_get(entry, "speed"));
        strncpy(e->date, json_string_value(json_object_get(entry, "date")), sizeof(e->date));
    }
    
    json_decref(root);
}

void save_stats() {
    json_t *root = json_object();
    
    for (size_t i = 0; i < player_count; i++) {
        PlayerStats *p = &players[i];
        json_t *stats_array = json_array();
        
        for (size_t j = 0; j < p->count; j++) {
            Stat *s = &p->stats[j];
            json_t *stat_obj = json_object();
            
            json_object_set_new(stat_obj, "date", json_string(s->date));
            json_object_set_new(stat_obj, "speed", json_real(s->speed));
            json_object_set_new(stat_obj, "accuracy", json_real(s->accuracy));
            json_object_set_new(stat_obj, "time", json_real(s->elapsed));
            json_object_set_new(stat_obj, "correct", json_integer(s->correct));
            json_object_set_new(stat_obj, "wrong", json_integer(s->wrong));
            json_object_set_new(stat_obj, "difficulty", json_integer(s->difficulty));
            
            json_array_append_new(stats_array, stat_obj);
        }
        
        json_object_set_new(root, p->nickname, stats_array);
    }
    
    if (json_dump_file(root, STATS_FILE, JSON_INDENT(2)) != 0) {
        fprintf(stderr, "Ошибка сохранения статистики в файл\n");
    }
    
    json_decref(root);
}

void save_leader() {
    json_t *root = json_array();
    
    for (size_t i = 0; i < leader_count; i++) {
        LeaderEntry *e = &leaderboard[i];
        json_t *entry = json_object();
        
        json_object_set_new(entry, "nickname", json_string(e->nickname));
        json_object_set_new(entry, "speed", json_real(e->speed));
        json_object_set_new(entry, "date", json_string(e->date));
        
        json_array_append_new(root, entry);
    }
    
    if (json_dump_file(root, LEADER_FILE, JSON_INDENT(2)) != 0) {
        fprintf(stderr, "Ошибка сохранения таблицы рекордов\n");
    }
    
    json_decref(root);
}

void show_stats_menu() {
    char nick[MAX_NICK_LEN];
    printf("Введите ник (Enter для всех): ");
    fgets(nick, MAX_NICK_LEN, stdin);
    nick[strcspn(nick,"\r\n")]='\0';
    if (strlen(nick)==0) {
        for (size_t i=0; i<player_count; i++) {
            PlayerStats *p=&players[i];
            double sum_sp=0, sum_acc=0;
            for (size_t j=0; j<p->count; j++) { sum_sp+=p->stats[j].speed; sum_acc+=p->stats[j].accuracy; }
            printf("%s: тестов %zu, средняя скорость %.1f, точность %.1f\n",
                   p->nickname, p->count, sum_sp/p->count, sum_acc/p->count);
        }
    } else {
        for (size_t i=0; i<player_count; i++) {
            if (strcmp(players[i].nickname, nick)==0) {
                for (size_t j=0; j<players[i].count; j++) {
                    Stat *s = &players[i].stats[j];
                    printf("#%zu %s: скорость %.1f, точность %.1f, время %.1f, верно %d, ошиб %d\n",
                           j+1, s->date, s->speed, s->accuracy, s->elapsed, s->correct, s->wrong);
                }
            }
        }
    }
}

void show_leaderboard() {
    printf("=== Рекорды ===\n");
    for (size_t i=0; i<leader_count; i++) {
        printf("%2zu. %s - %.1f зн/мин (%s)\n",
               i+1, leaderboard[i].nickname, leaderboard[i].speed, leaderboard[i].date);
    }
}