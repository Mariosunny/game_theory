#include <stdio.h>
#include <stdlib.h>
#include "reduce.h"

int rows;
int columns;


int reduce_columns(float **matrix, char *dominated_columns, char *dominated_rows, int size) {

	int col1, col2, col3, row, dominated;
	float a, b, c;
	float value;
	float upper, lower;

	upper = 1.0;
	lower = 0.0;

	if(columns == 1) {

		return 0;
	}

	for(col1 = 0; col1 < size; col1++) {
		for(col2 = 0; col2 < size; col2++) {
			for(col3 = 0; col3 < size; col3++) {

				if(col1 == col2 || col1 == col3 || col2 == col3) {

					continue;
				}

				if(dominated_columns[col3]) {

					continue;
				}

				dominated = 1;

				for(row = 0; row < size; row++) {

					if(dominated_rows[row]) {

						continue;
					}

					a = matrix[row][col1];
					b = matrix[row][col2];
					c = matrix[row][col3];

					if(a == b && a > c) {

						dominated = 0;
						break;
					}

					value = (c - b)/(a - b);
					
					if(a - b >= 0) {

						if(value < 0) {

							dominated = 0;
							break;
						}

						if(value < upper) {

							upper = value;
						}
					}
					else {

						if(value > 1) {

							dominated = 0;
							break;
						}

						if(value > lower) {

							lower = value;
						}
					}

					if(upper < lower) {

						dominated = 0;
						break;
					}
				}

				if(dominated) {

					dominated_columns[col3] = 1;
					columns--;
					return 1;
				}
			}
		}
	}

	return 0;
}

int reduce_rows(float **matrix, char *dominated_columns, char *dominated_rows, int size) {

	int row1, row2, row3, col, dominated;
	float a, b, c;
	float value;
	float upper, lower;

	upper = 1.0;
	lower = 0.0;

	if(rows == 1) {

		return 0;
	}

	for(row1 = 0; row1 < size; row1++) {
		for(row2 = 0; row2 < size; row2++) {
			for(row3 = 0; row3 < size; row3++) {

				if(row1 == row2 || row1 == row3 || row2 == row3) {

					continue;
				}

				if(dominated_rows[row3]) {

					continue;
				}

				dominated = 1;

				for(col = 0; col < size; col++) {

					if(dominated_columns[col]) {

						continue;
					}

					a = matrix[row1][col];
					b = matrix[row2][col];
					c = matrix[row3][col];

					if(a == b && a > c) {

						dominated = 0;
						break;
					}

					value = (c - b)/(a - b);
					
					if(a - b >= 0) {

						if(value < 0) {

							dominated = 0;
							break;
						}

						if(value < upper) {

							upper = value;
						}
					}
					else {

						if(value > 1) {

							dominated = 0;
							break;
						}

						if(value > lower) {

							lower = value;
						}
					}

					if(upper < lower) {

						dominated = 0;
						break;
					}
				}

				if(dominated) {

					dominated_rows[row3] = 1;
					rows--;
					return 1;
				}
			}
		}
	}

	return 0;
}

void print_matrix(float **matrix, char *dominated_columns, char *dominated_rows, int size) {

	int row, column;

	for(row = 0; row < size; row++) {

		if(dominated_rows[row]) {

			continue;
		}

		for(column = 0; column < size; column++) {

			if(dominated_columns[column]) {

				continue;
			}

			printf("%f ", matrix[row][column]);
		}

		printf("\n");
	}
}

float get_value(float **matrix, char *dominated_columns, char *dominated_rows, int size) {

	int row, column;
	int i = 0;
	float f[4];

	for(row = 0; row < size; row++) {

		if(dominated_rows[row]) {

			continue;
		}

		for(column = 0; column < size; column++) {

			if(dominated_columns[column]) {

				continue;
			}

			f[i] = matrix[row][column];
			i++;

			if(i == 4) {

				break;
			}
		}

		if(i == 4) {

			break;
		}
	}

	if(i == 1) {

		return f[0];
	}

	return (f[0]*f[3] - f[1]*f[2])/(f[0] - f[1] + f[3] - f[2]);
}

void fill_result(float **matrix1, float **matrix2, char *dominated_columns, char *dominated_rows, int size, result *result) {

	int row, column;
	int i = 0;
	float f1[4];
	float f2[4];
	int s[4];
	int t[4];

	for(row = 0; row < size; row++) {

		if(dominated_rows[row]) {

			continue;
		}

		for(column = 0; column < size; column++) {

			if(dominated_columns[column]) {

				continue;
			}

			f1[i] = matrix1[row][column];
			f2[i] = matrix2[row][column];
			s[i] = row;
			t[i] = column;
			i++;

			if(i == 4 || rows == 1) {

				break;
			}
		}

		if(i == 4) {

			break;
		}
	}

	if(rows == 1) {

		result->v1 = f1[0];
		result->v2 = f2[0];
		result->p1 = 1;
		result->q1 = 1;
		result->s1 = s[0];
		result->t1 = t[0];
	}
	else {

		result->v1 = (f1[0]*f1[3] - f1[1]*f1[2])/(f1[0] - f1[1] + f1[3] - f1[2]);
		result->v2 = (f2[0]*f2[3] - f2[1]*f2[2])/(f2[0] - f2[1] + f2[3] - f2[2]);
		result->p1 = (f1[3] - f1[2])/(f1[0] - f1[1] + f1[3] - f1[2]);
		result->p2 = 1 - result->p1;
		result->q1 = (f2[3] - f2[2])/(f2[0] - f2[1] + f2[3] - f2[2]);
		result->q2 = 1 - result->q1;
		result->s1 = s[0];
		result->s2 = s[3];
		result->t1 = t[0];
		result->t2 = t[3];
	}

	result->v = (result->v1 + result->v2)/2.0;
}

void fill_overall_values(float **matrix1, float **matrix2, int size, result *result) {

	float min_value = (matrix1[0][0] + matrix2[0][0])/2.0;
	int i, j;
	int min_row = 0;
	int min_column = 0;
	float value;

	for(i = 0; i < size; i++) {

		for(j = 0; j < size; j++) {

			value = (matrix1[i][j] + matrix2[i][j])/2.0;

			if(min_value > value) {

				min_value = value;
				min_row = i;
				min_column = j;
			}
		}
	}

	result->v_min = min_value;
	result->s_min = min_row;
	result->t_min = min_column;
}

void get_values(float **matrix1, float **matrix2, int size, result *result) {

	int i;
	int column_reduced, row_reduced;
	char *dominated_columns = malloc(size*sizeof(char));
	char *dominated_rows = malloc(size*sizeof(char));
	rows = size;
	columns = size;

	for(i = 0; i < size; i++) {

		dominated_columns[i] = 0;
		dominated_rows[i] = 0;
	}

	column_reduced = 0;
	row_reduced = 1;

	while(1) {

		printf("%f%% done\n", 100.0 - 100.0*(rows + columns)/(size*2));

		if(column_reduced) {

			row_reduced = reduce_rows(matrix2, dominated_columns, dominated_rows, size);

			if(!row_reduced) {

				column_reduced = reduce_columns(matrix1, dominated_columns, dominated_rows, size);
			}
		}
		else if(row_reduced) {

			column_reduced = reduce_columns(matrix1, dominated_columns, dominated_rows, size);

			if(!column_reduced) {

				row_reduced = reduce_rows(matrix2, dominated_columns, dominated_rows, size);
			}
		}

		if(row_reduced == 0 && column_reduced == 0){

			break;
		}
	}

	fill_result(matrix1, matrix2, dominated_columns, dominated_rows, size, result);
	fill_overall_values(matrix1, matrix2, size, result);

	free(dominated_rows);
	free(dominated_columns);
}
