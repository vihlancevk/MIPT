import sys
import pandas as pd


if __name__ == '__main__':
	if len(sys.argv) != 2:
		print('Command must has next syntaxis:\n\tpython3 parallel.py [np]')
		exit()

	n = int(sys.argv[1])

	df = pd.DataFrame({'x': [], 'time': [], 'value': []})
	for i in range(n):
		df_i = pd.read_csv(f'logs/process_log_{i}_of_{n}')
		df = pd.concat([df, df_i])	
	df.reset_index(drop=True, inplace=True)

	print(df)
