#ifndef CALCULATE_H
#define CALCULATE_H

typedef struct result {
	float client1_value;
	float client2_value;
} result;

int sort_requests_by_sent(const void *a, const void *b);
void get_payoffs(int *play1, int *play2, int size, int network_latency,  int **requests_made, float payoffs[2]);
void get_result(int number_of_turns, int number_of_strategies, int number_of_servers, int delay, int network_latency, int **requests_made, result *result);

#endif