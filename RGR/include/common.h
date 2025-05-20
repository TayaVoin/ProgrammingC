#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <jansson.h>

#define MAX_TEXT_LEN 512
#define MAX_NICK_LEN 64
#define STATS_FILE "stats.json"
#define LEADER_FILE "leaderboard.json"
#define MAX_ENTRIES 100

typedef enum { EASY, MEDIUM, HARD, ASDF, JKL, CUSTOM, MINECRAFT } Diff;

typedef struct {
    char date[20];
    double speed;
    double accuracy;
    double elapsed;
    int correct;
    int wrong;
    Diff difficulty;
} Stat;

typedef struct {
    char nickname[MAX_NICK_LEN];
    Stat *stats;
    size_t count;
} PlayerStats;

typedef struct {
    char nickname[MAX_NICK_LEN];
    double speed;
    char date[20];
} LeaderEntry;

extern const char *texts_easy[];
extern const char *texts_medium[];
extern const char *texts_hard[];
extern const char *texts_asdf[];
extern const char *texts_jkl[];
extern const char *texts_minecraft[];

extern char **texts_custom;
extern size_t custom_count;
extern PlayerStats *players;
extern size_t player_count;
extern LeaderEntry leaderboard[];
extern size_t leader_count;
extern Diff current_diff;

void current_timestamp(char *buf, size_t sz);
void clear_buf();

#endif