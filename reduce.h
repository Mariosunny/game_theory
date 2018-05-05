#ifndef REDUCE_H
#define REDUCE_H

int reduce_columns(float **matrix, char *dominated_columns, char *dominated_rows, int size);
int reduce_rows(float **matrix, char *dominated_columns, char *dominated_rows, int size);
void print_matrix(float **matrix, char *dominated_columns, char *dominated_rows, int size);
float get_value(float **matrix, char *dominated_columns, char *dominated_rows, int size);
void get_values(float **matrix1, float **matrix2, int size, float values[2]);

#endif