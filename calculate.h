#ifndef CALCULATE_H
#define CALCULATE_H
#include "reduce.h"

typedef struct play {
	int delay;
	int server;
} play;

int sort_requests_by_sent(const void *a, const void *b);
void get_payoffs(play *plays1, play *plays2, int number_of_servers, int number_of_turns, int network_latency, int **requests_made, float payoffs[2]);
void get_result(int number_of_turns, int number_of_strategies, int number_of_servers, int delay, int network_latency, int **requests_made, result *result);

#endif