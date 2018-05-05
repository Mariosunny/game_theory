#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calculate.h"
#include "reduce.h"

typedef struct request {
	int id;
	int client;
	int duration;
	int made;
	int sent;
	int received;
	int returned;
} request;

int sort_requests_by_sent(const void *a, const void *b) {

	request *req1 = (request*)a;
	request *req2 = (request*)b;
	int value = req1->sent - req2->sent;

	if(value == 0) {

		return req1->client - req2->client;
	}

	return value;
}

void get_payoffs(int *play1, int *play2, int size, int network_latency, int **requests_made, float payoffs[2]) {

	request requests[size*2];
	request *req;
	int i;
	int client;
	int time = 0;
	payoffs[0] = 0;
	payoffs[1] = 0;

	for(i = 0; i < size*2; i++) {

		client = size <= i ? 2: 1;
		req = &requests[i];
		req->id = i;
		req->client = client;
		req->duration = requests_made[i % size][0];
		req->made = requests_made[i % size][1];
		req->sent = req->made + (client == 1 ? play1[i % size]: play2[i % size]);
		requests[i] = *req;
	}

	qsort(requests, size*2, sizeof(request), sort_requests_by_sent);

	for(i = 0; i < size*2; i++) {

		req = &requests[i];
		req->received = time;
		req->returned = req->received + req->duration;
		time += req->duration;
	}

	for(i = 0; i < size*2; i++) {

		req = &requests[i];
		payoffs[req->client - 1] += req->returned - req->made + 2*network_latency;
	}

	for(i = 0; i <= 1; i++) {

		payoffs[i] = payoffs[i]/size;
	}
}

void get_result(int number_of_turns, int number_of_strategies, int number_of_servers, int delay, int network_latency, int **requests_made, result *result) {

	int i, j, k;
	int size = (int) pow(number_of_strategies, number_of_turns);
	int *play1 = malloc(number_of_turns*sizeof(int));
	int *play2 = malloc(number_of_turns*sizeof(int));
	float payoffs[2];
	float values[2];
	float **matrix1 = malloc(size*sizeof(float*));
	float **matrix2 = malloc(size*sizeof(float*));

	for(i = 0; i < size; i++) {

		matrix1[i] = malloc(size*sizeof(float));
		matrix2[i] = malloc(size*sizeof(float));
	}

	printf("Computing %dx%d matrix...\n", size, size);

	for(i = 0; i < size; i++) {

		for(j = 0; j < size; j++) {

			for(k = 0; k < number_of_turns; k++) {

				play1[k] = delay*((i/((int)pow(number_of_strategies, k))) % number_of_strategies);
			}

			for(k = 0; k < number_of_turns; k++) {

				play2[k] = delay*((j/((int)pow(number_of_strategies, k))) % number_of_strategies);
			}
			get_payoffs(play1, play2, number_of_turns, network_latency, requests_made, payoffs);
			matrix1[i][j] = payoffs[0];
			matrix2[i][j] = payoffs[1];
		}
	}

	free(play1);
	free(play2);

	printf("Reducing matrix...\n");
	get_values(matrix1, matrix2, size, values);

	for(i = 0; i < size; i++) {

		free(matrix1[i]);
		free(matrix2[i]);
	}

	free(matrix1);
	free(matrix2);

	result->client1_value = values[0];
	result->client2_value = values[1];
}
