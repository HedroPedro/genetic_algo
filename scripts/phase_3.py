from subprocess import run
from sys import argv
import pathlib
import shutil
from helper import write_ini_file

INI_DICT = {
	'EXPERIMENT': '',
	'TSV': 'tomtom_out/tomtom.tsv',
	'SHELL': 'run.sh',
	'ITER': 0,
	'POP': 0,
	'MODE': 'RANDOM'
}

N_REPS = 5
CHK = pathlib.Path('checkpoint.chk')


def run_alg(target: str):
	result = run(['./alg', target], capture_output=True, text=True)
	if result.returncode != 0:
		print(f'!! alg failed on {target}')
		print(result.stdout)
		print(result.stderr)
	return result


def run_tests(test_configs: list[tuple[int, int]], mode='GENETIC'):
	csv_prefix = 'random_generations' if mode == 'RANDOM' else 'generations'

	for pop, iters in test_configs:
		csv_name = f'{csv_prefix}_{iters}_{pop}.csv'

		for rep in range(1, N_REPS + 1):
			print(f'== MODE={mode} POP={pop} ITER={iters} rep={rep}/{N_REPS} ==')

			INI_DICT['EXPERIMENT'] = argv[1]
			INI_DICT['POP'] = pop
			INI_DICT['ITER'] = iters
			INI_DICT['MODE'] = mode

			ini_path = f'test_{mode.lower()}_pop{pop}_iter{iters}.ini'
			write_ini_file(ini_path, INI_DICT)

			run_alg(ini_path)

			if CHK.exists():
				print(f'!! rep {rep} exited without completing (checkpoint left behind), skipping rename')
				continue

			src = pathlib.Path(csv_name)
			if not src.exists():
				print(f'!! expected output {csv_name} not found after rep {rep}')
				continue

			dst = pathlib.Path(f'{csv_prefix}_{iters}_{pop}_{rep}.csv')
			shutil.move(src, dst)
			print(f'saved -> {dst}')


if __name__ == '__main__':
	if CHK.exists():
		print(f'Found leftover checkpoint {CHK}, resuming before starting new tests...')
		run_alg(str(CHK))

	print('!! Testing random algorithm phase 3')
	run_tests([], mode='GENETIC')
