from subprocess import run
from sys import argv
import pathlib
import shutil
from csv import DictReader
from helper import write_ini_file

INI_DICT = {
	'EXPERIMENT': '',
	'TSV': 'tomtom_out/tomtom.tsv',
	'SHELL': 'run.sh',
	'ITER': 100,
	'POP': 100,
	'MODE': 'RANDOM',
	'BUDGET': 0,
}

CHK = pathlib.Path('checkpoint.chk')

def run_alg(target: str):
	result = run(['./alg', target], capture_output=True, text=True)
	if result.returncode != 0:
		print(f'!! alg failed on {target}')
		print(result.stdout)
		print(result.stderr)
	return result


def run_tests(mode='RANDOM'):
	csv_prefix = 'random_generations' if mode == 'RANDOM' else 'generations'

	csvs = list(pathlib.Path().cwd().glob('generations_100_100_*.csv'))
	rep = 1

	csv_name = f'{csv_prefix}_100_100.csv'

	for csv in csvs:
		print(f'== MODE={mode} POP=100 ITER=100 BUDGET={budget} rep={rep}/{len(csvs)+1} ==')

		csv_dict = dict()
		budget = 0

		with open(csv) as csv_file:
			reader = DictReader(csv_file, delimiter=';')
			for row in reader:
				budget += int(row['Budget'])

		INI_DICT['EXPERIMENT'] = argv[1]
		INI_DICT['BUDGET'] = budget

		ini_path = f'test_{mode.lower()}_pop100_iter100.ini'
		write_ini_file(ini_path, INI_DICT)

		run_alg(ini_path)

		if CHK.exists():
			print(f'!! rep {rep} exited without completing (checkpoint left behind), skipping rename')
			continue

		src = pathlib.Path(csv_name)
		if not src.exists():
			print(f'!! expected output {csv_name} not found after rep {rep}')
			continue

		dst = pathlib.Path(f'{csv_prefix}_100_100_{rep}.csv')
		shutil.move(src, dst)
		print(f'saved -> {dst}')


if __name__ == '__main__':
	if CHK.exists():
		print(f'Found leftover checkpoint {CHK}, resuming before starting new tests...')
		run_alg(str(CHK))

	print('!! Testing genetic algorithm phase 4')
	run_tests()