#ifndef REDUCE_H
#define REDUCE_H

typedef struct result {
	float v;
	float v_min;
	float v1;
	float v2;
	float p1;
	float p2;
	float q1;
	float q2;
	int delay;
	int s_min;
	int t_min;
	int s1;
	int s2;
	int t1;
	int t2;
	int size;
	int servers;
	int strategies;
	int requests;
} result;

int reduce_columns(float **matrix, char *dominated_columns, char *dominated_rows, int size);
int reduce_rows(float **matrix, char *dominated_columns, char *dominated_rows, int size);
void print_matrix(float **matrix, char *dominated_columns, char *dominated_rows, int size);
void fill_result(float **matrix1, float **matrix2, char *dominated_columns, char *dominated_rows, int size, result *result);
float get_value(float **matrix, char *dominated_columns, char *dominated_rows, int size);
void fill_overall_values(float **matrix1, float **matrix2, int size, result *result);
void get_values(float **matrix1, float **matrix2, int size, result *result);

#endif