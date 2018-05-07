#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "reduce.h"
#include "calculate.h"

#define NUMBER_OF_TURNS 4
#define NETWORK_LATENCY 10
#define DELAY 20

int REQUESTS[NUMBER_OF_TURNS][2] = {
	{85, 0},
	{10, 20},
	{115, 45},
	{50, 60}
};

void print_result(int **requests_made, result *result) {

	int i, delay, server;

	for(i = 0; i < result->requests; i++) {

		printf("(duration: %d, arrival time: %d)\n", requests_made[i][0], requests_made[i][1]);
	}

	printf("\n");
	printf("Matrix size: %dx%d\n", result->size, result->size);
	printf("Requests: %d\n", result->requests);
	printf("Strategies: %d\n", result->strategies);
	printf("Servers: %d\n", result->servers);
	printf("Value: %f\n", result->v);
	printf("Min value: %f\n", result->v_min);
	printf("Min player 1 strategy: ");

	for(i = 0; i < result->requests; i++) {

		delay = result->delay*((result->s_min/((int)pow(result->strategies*result->servers, i))) % result->strategies);
		server = (result->s_min/((int)pow(result->strategies*result->servers, i))) % result->servers;
		printf("(delay %d, server %d) ", delay, server);
	}
	printf("\n");
	printf("Min player 2 strategy: ");

	for(i = 0; i < result->requests; i++) {

		delay = result->delay*((result->t_min/((int)pow(result->strategies*result->servers, i))) % result->strategies);
		server = (result->t_min/((int)pow(result->strategies*result->servers, i))) % result->servers;
		printf("(delay %d, server %d) ", delay, server);
	}
	printf("\n\n");
	printf("Player 1:\n");
	printf("\tvalue: %f\n", result->v1);
	printf("\tstrategy: ");

	for(i = 0; i < result->requests; i++) {

		delay = result->delay*((result->s1/((int)pow(result->strategies*result->servers, i))) % result->strategies);
		server = (result->s1/((int)pow(result->strategies*result->servers, i))) % result->servers;
		printf("(delay %d, server %d) ", delay, server);
	}
	printf("\n");
	printf("Player 2:\n");
	printf("\tvalue: %f\n", result->v2);
	printf("\tstrategy: ");

	for(i = 0; i < result->requests; i++) {

		delay = result->delay*((result->t1/((int)pow(result->strategies*result->servers, i))) % result->strategies);
		server = (result->t1/((int)pow(result->strategies*result->servers, i))) % result->servers;
		printf("(delay %d, server %d) ", delay, server);
	}
	printf("\n");
}

int main() {

	srand(time(NULL));

	result result;
	int **requests_made = malloc(NUMBER_OF_TURNS*sizeof(int*));
	int i;
	int time;
	int delay;

	for(i = 0; i < NUMBER_OF_TURNS; i++) {

		requests_made[i] = malloc(2*sizeof(int));
	}

	for(i = 0; i < NUMBER_OF_TURNS; i++) {

		requests_made[i][0] = REQUESTS[i][0];
		requests_made[i][1] = REQUESTS[i][1];
	}

	get_result(NUMBER_OF_TURNS, 2, 3, DELAY, NETWORK_LATENCY, requests_made, &result);
	print_result(requests_made, &result);

	for(i = 0; i < NUMBER_OF_TURNS; i++) {

		free(requests_made[i]);
	}

	free(requests_made);

	return 0;
}