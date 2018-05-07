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
	int received;
	int sent;
	int returned;
	int server;
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

void get_payoffs(play *plays1, play *plays2, int number_of_servers, int number_of_turns, int network_latency, int **requests_made, float payoffs[2]) {

	int size = number_of_turns;
	request requests[size*2];
	request *req;
	request temp;
	int i;
	int client;
	int *servers = malloc(number_of_servers*sizeof(int));
	payoffs[0] = 0;
	payoffs[1] = 0;
	play play;

	for(i = 0; i < number_of_servers; i++) {

		servers[i] = 0;
	}

	for(i = 0; i < size*2; i++) {

		client = size <= i ? 1: 0;
		play = client == 1 ? plays1[i % size]: plays2[i % size];
		req = &requests[i];
		req->id = i;
		req->client = client;
		req->duration = requests_made[i % size][0];
		req->made = requests_made[i % size][1];
		req->sent = req->made + play.delay;
		req->received = req->sent + network_latency;
		req->server = play.server;
		requests[i] = *req;
	}

	qsort(requests, size*2, sizeof(request), sort_requests_by_sent);

	for(i = 2; i < size*2; i+= 4) {

		temp = requests[i];
		requests[i] = requests[i+1];
		requests[i + 1] = temp;
	}

	for(i = 0; i < size*2; i++) {

		req = &requests[i];

		if(req->received > servers[req->server]) {

			servers[req->server] = req->received;
		}

		servers[req->server] += req->duration;
		req->returned = servers[req->server] + network_latency;
	}

	for(i = 0; i < size*2; i++) {

		req = &requests[i];
		payoffs[req->client] += req->returned - req->made;
	}

	for(i = 0; i <= 1; i++) {

		payoffs[i] = payoffs[i]/size;
	}

	free(servers);
}

void get_result(int number_of_turns, int number_of_strategies, int number_of_servers, int delay, int network_latency, int **requests_made, result *result) {

	int i, j, k, s;
	int size = (int) pow(number_of_strategies*number_of_servers, number_of_turns);
	play *plays1 = malloc(number_of_turns*sizeof(play));
	play *plays2 = malloc(number_of_turns*sizeof(play));
	float payoffs[2];
	float **matrix1 = malloc(size*sizeof(float*));
	float **matrix2 = malloc(size*sizeof(float*));

	for(i = 0; i < size; i++) {

		matrix1[i] = malloc(size*sizeof(float));
		matrix2[i] = malloc(size*sizeof(float));
	}

	printf("Calculating %dx%d matrix...\n", size, size);

	for(i = 0; i < size; i++) {

		for(j = 0; j < size; j++) {

			for(k = 0; k < number_of_turns; k++) {

				plays1[k].delay = delay*((i/((int)pow(number_of_strategies*number_of_servers, k))) % number_of_strategies);
				plays1[k].server = (i/((int)pow(number_of_strategies*number_of_servers, k))) % number_of_servers;
			}

			for(k = 0; k < number_of_turns; k++) {

				plays2[k].delay = delay*((j/((int)pow(number_of_strategies*number_of_servers, k))) % number_of_strategies);
				plays2[k].server = (j/((int)pow(number_of_strategies*number_of_servers, k))) % number_of_servers;
			}

			get_payoffs(plays1, plays2, number_of_servers, number_of_turns, network_latency, requests_made, payoffs);
			matrix1[i][j] = payoffs[0];
			matrix2[i][j] = payoffs[1];
		}
	}

	free(plays1);
	free(plays2);

	printf("Reducing matrix...\n");
	get_values(matrix1, matrix2, size, result);

	result->delay = delay;
	result->size = size;
	result->servers = number_of_servers;
	result->strategies = number_of_strategies;
	result->requests = number_of_turns;

	for(i = 0; i < size; i++) {

		free(matrix1[i]);
		free(matrix2[i]);
	}

	free(matrix1);
	free(matrix2);
}
