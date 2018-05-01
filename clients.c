#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

#define NUMBER_OF_TURNS 2
#define NUMBER_OF_STRATEGIES 2
#define BASE_DELAY 50

int REQUESTS[NUMBER_OF_TURNS][2] = {
	{100, 0},
	{50, 50}
};

typedef struct request {
	int id;
	int client;
	int duration;
	int health;
	int sent;
	int received;
} request;

struct node;

typedef struct node {
	request *request;
	struct node *next;
} node;

void delay(int milliseconds) {
	long pause;
	clock_t now,then;

	pause = milliseconds*(CLOCKS_PER_SEC/1000);
	now = then = clock();
	while( (now-then) < pause ) {
		
		now = clock();
	}
}

request *peek(node *head) {

	if(head) {

		return head->request;
	}

	return NULL;
}

request *pop(node **head) {

	node *temp = *head;
	request *request;

	if(temp) {

		request = temp->request;
		*head = temp->next;
		temp->next = NULL;
		free(temp);
		return request;
	}

	exit(0);
}

void push(node **head, request *request) {

	node *new_node = malloc(sizeof(node));
	new_node->request = request;
	new_node->next = NULL;
	node *temp = (*head);

	if(temp == NULL) {

		(*head) = new_node;
		return;
	}
	
	while(temp->next) {

		temp = temp->next;
	}

	temp->next = new_node;
}

int network_latency(int time) {

	return 10;
}

node *generate_requests(int client, int play[NUMBER_OF_TURNS]) {

	node *head = NULL;
	request *request;
	int i;
	int arrival_time = 0;

	for(i = 0; i < NUMBER_OF_TURNS; i++) {

		arrival_time += REQUESTS[i][1];
		request = malloc(sizeof(request));
		request->id = i + (client - 1)*NUMBER_OF_TURNS;
		request->client = client;
		request->duration = REQUESTS[i][0];
		request->health = arrival_time + play[i];
		push(&head, request);
	}

	return head;
}

void process_client(node **client, node **network_inbound, int time) {

	node *current = *client;
	node *last = NULL;
	request *request;

	while(current) {

		request = current->request;

		if(request->health == 0) {

			request->health = network_latency(time);
			request->sent = time;
			push(network_inbound, request);

			if(last) {

				last->next = current->next;
				free(current);
			}
			else{

				pop(client);
			}
		}
		else {
		
			request->health--;
			last = current;
		}
		current = current->next;
	}
}

void process_network_inbound(node **network_inbound, node **server) {

	node *current = *network_inbound;
	node *last = NULL;
	request *request;

	while(current) {

		request = current->request;

		if(request->health == 0) {

			request->health = request->duration;
			push(server, request);

			if(last) {

				last->next = current->next;
				free(current);
			}
			else{

				pop(network_inbound);
			}
		}
		else {
			request->health--;
			last = current;
		}
		current = current->next;
	}
}

void process_server(node **server, node **network_outbound, int time) {

	request *request;

	if(*server) {

		request = (*server)->request;

		if(request->health == 0) {

			request->health = network_latency(time);
			pop(server);
			push(network_outbound, request);
			return;
		}

		request->health--;
	}
}

void process_network_outbound(node **network_outbound, node **client1_done, node **client2_done, int time) {

	node *current = *network_outbound;
	node *last = NULL;
	request *request;

	while(current) {

		request = current->request;

		if(request->health == 0) {

			request->received = time;

			if(request->client == 1) {

				push(client1_done, request);
			}
			else {

				push(client2_done, request);
			}

			if(last) {

				last->next = current->next;
				free(current);
			}
			else{

				pop(network_outbound);
			}
		}
		else {

			request->health--;
			last = current;
		}
		current = current->next;
	}
}

void print_requests(node *requests) {

	node *current = requests;
	request *request;

	if(!requests) {

		return;
	}

	while(current) {
		request = current->request;
		printf("id: %d, client: %d, duration: %d, health: %d, sent: %d, received: %d\n", request->id, request->client, request->duration, request->health, request->sent, request->received);
		current = current->next;
	}

	printf("\n");
}

float get_average_latency(node **client_done) {

	request *request;
	node *current = *client_done;
	float sum = 0;

	while(current) {

		request = current->request;
		sum += request->received - request->sent;
		current = current->next;
	}

	return sum/NUMBER_OF_TURNS;
}

void free_requests(node *head) {

	while(head) {

		free(head->request);
		head = head->next;
	}

	return;
}

void get_payoffs(int play1[NUMBER_OF_TURNS], int play2[NUMBER_OF_TURNS], float payoffs[2]) {

	int time = 0;
	node *server = NULL;
	node *network_inbound = NULL;
	node *network_outbound = NULL;
	node *client1_done = NULL;
	node *client2_done = NULL;
	node *client1 = generate_requests(1, play1);
	node *client2 = generate_requests(2, play2);

	while(client1 || client2 || network_inbound || server || network_outbound) {

		process_client(&client1, &network_inbound, time);
		process_client(&client2, &network_inbound, time);
		process_network_inbound(&network_inbound, &server);
		process_server(&server, &network_outbound, time);
		process_network_outbound(&network_outbound, &client1_done, &client2_done, time);
		time++;
	}

	free_requests(client1_done);
	free_requests(client2_done);

	payoffs[0] = get_average_latency(&client1_done);
	payoffs[1] = get_average_latency(&client2_done);
}

void run() {

	int i, j, k;
	int play1[NUMBER_OF_TURNS];
	int play2[NUMBER_OF_TURNS];
	int NUMBER_OF_PURE_STRATEGIES = (int) pow(NUMBER_OF_STRATEGIES, NUMBER_OF_TURNS);
	float payoffs[2];
	float payoff_matrix1[NUMBER_OF_PURE_STRATEGIES][NUMBER_OF_PURE_STRATEGIES];
	float payoff_matrix2[NUMBER_OF_PURE_STRATEGIES][NUMBER_OF_PURE_STRATEGIES];
	int count = 0;
	FILE *file1;
	FILE *file2;

	file1 = fopen("results1", "w");
	file2 = fopen("results2", "w");

	printf("Computing %dx%d matrix...\n", NUMBER_OF_PURE_STRATEGIES, NUMBER_OF_PURE_STRATEGIES);

	for(i = 0; i < NUMBER_OF_PURE_STRATEGIES; i++) {

		for(j = 0; j < NUMBER_OF_PURE_STRATEGIES; j++) {

			for(k = 0; k < NUMBER_OF_TURNS; k++) {

				play1[k] = BASE_DELAY*((i/((int)pow(NUMBER_OF_STRATEGIES, k))) % NUMBER_OF_STRATEGIES);
			}

			for(k = 0; k < NUMBER_OF_TURNS; k++) {

				play2[k] = BASE_DELAY*((j/((int)pow(NUMBER_OF_STRATEGIES, k))) % NUMBER_OF_STRATEGIES);
			}
			get_payoffs(play1, play2, payoffs);
			payoff_matrix1[i][j] = payoffs[0];
			payoff_matrix2[i][j] = payoffs[1];
			printf("%f  ", payoff_matrix1[i][j]);
		}

		printf("\n");

		fwrite(payoff_matrix1[i], sizeof(float), NUMBER_OF_PURE_STRATEGIES, file1);
		fwrite(payoff_matrix2[i], sizeof(float), NUMBER_OF_PURE_STRATEGIES, file2);
	}

	fclose(file1);
	fclose(file2);
}


int main() {

	run();

	return 0;
}