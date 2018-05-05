player1 = [
	[0, 1, 5],
	[1, 5, 4],
	[3, 2, 6]
]

player2 = [
	[2, 5, 4],
	[5, 7, 8],
	[3, 4, 7]
]

MAX_DEPTH = 8


def reduce_column(matrix):

	for column1 in range(len(matrix[0])):
		for column2 in range(len(matrix[0])):
			for column3 in range(len(matrix[0])):

				if column1 == column2 or column2 == column3 or column1 == column3:

					continue

				print(column1, column2, column3)
				test_columns(matrix, column1, column2, column3, 1, 1, 0)


def test_columns(matrix, column1, column2, column3, numerator, denominator, depth):

	values = []

	p = numerator/denominator

	for y in range(len(matrix)):

		values.append(matrix[y][column1]*p + matrix[y][column2]*(1-p) <= matrix[y][column3])

	if all(values):

		print("Remove column " + str(column3) + " using p = " + str(numerator) + "/" + str(denominator) + " with columns " + str(column1) + ", " + str(column2))

		return True

	if depth < MAX_DEPTH:

		return test_columns(matrix, column1, column2, column3, 2*numerator+1, denominator*2, depth + 1) or test_columns(matrix, column1, column2, column3, 2*numerator-1, denominator*2, depth + 1)

	return False

reduce_column(player2)