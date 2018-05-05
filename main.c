#include <stdlib.h>
#include <stdio.h>
#include "reduce.h"
#include "calculate.h"

#define NUMBER_OF_TURNS 4
#define NETWORK_LATENCY 10
#define DELAY 25

int REQUESTS[NUMBER_OF_TURNS][2] = {
	{100, 0},
	{50, 50},
	{250, 100},
	{80, 105}
};

int main() {

	result result;
	int **requests_made = malloc(NUMBER_OF_TURNS*sizeof(int*));
	int i;

	for(i = 0; i < NUMBER_OF_TURNS; i++) {

		requests_made[i] = malloc(2*sizeof(int));
	}

	for(i = 0; i < NUMBER_OF_TURNS; i++) {

		requests_made[i][0] = REQUESTS[i][0];
		requests_made[i][1] = REQUESTS[i][1];
	}

	get_result(NUMBER_OF_TURNS, 6, 1, DELAY, NETWORK_LATENCY, requests_made, &result);
	printf("player 1: %f\nplayer 2: %f\n", result.client1_value, result.client2_value);

	for(i = 0; i < NUMBER_OF_TURNS; i++) {

		free(requests_made[i]);
	}

	free(requests_made);

	return 0;
}