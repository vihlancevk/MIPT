#!/usr/bin/env python3

import sys

from random import randint


if __name__ == '__main__':
	n: int = int(sys.argv[1])
	file_name: str = sys.argv[2]
	
	with open(file_name, 'w') as f:
		low_bound: int = 1
		high_bound: int = 100

		for i in range(n):
			for j in range(n):
				number: int = randint(low_bound, high_bound)
				f.write(str(number) + ' ' if j < n - 1 else '')
