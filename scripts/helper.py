def write_ini_file(fp, ini_dict: dict):
	with open(fp, 'w') as file:
		for k, v in ini_dict.items():
			file.write(f'{k}={v}\n')
	