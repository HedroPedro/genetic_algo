# Genetic Algorithm
> A genetic algorithm to find the best parameters in pipelines for motif discovery

# Requirements
- C++ compiler
- Your favorite text editor

# How to compile
To compile the genetic algorithm simply type the command `make`. This will generate the algorithm in a file named **alg**

# How to use
This program accepts either a configuration (.ini) file or a checkpoint (.chk) file generated automatically by a previous run.

```bash
./alg file.ini
```

Or, to resume a previously interrupted run:

```bash
./alg checkpoint.chk
```

Run `./alg -h` at any time to see this same information from the command line.

## Configuration file parameters

| Key | Description | Default |
|-----|-------------|---------|
| `EXPERIMENT` | Path to the `.bam` file | **Required** |
| `SHELL` | Path to the shell pipeline run after MACS3 | **Required** |
| `TSV` | Path to the TOMTOM-generated `.tsv` | **Required** |
| `EXTRA` | Extra MACS3 parameters (e.g. genome size, `--broad`) | *(none)* |
| `MACS_DIR` | Name of the MACS3 output directory | `res` |
| `MODE` | `GENETIC` or `RANDOM` | `GENETIC` |
| `POP` | Population size (genetic) / points per iteration (random) | `100` |
| `ITER` | Number of generations / iterations | `100` |
| `BUDGET` | Fixed evaluation budget for Random Search. `0` = calculated as `POP × ITER` | `0` |
| `CHECKPOINT` | Path where the checkpoint file will be written | `checkpoint.chk` |

### Example configuration file
```ini
EXPERIMENT=experiment.bam
SHELL=run.sh
TSV=tomtom_out/tomtom.tsv
MACS_DIR=res
```

## Checkpointing

A checkpoint file (`.chk`) is created automatically the moment a run starts, using the path given by `CHECKPOINT` (default `checkpoint.chk`). It is updated after every generation (Genetic mode) or every batch of `POP` evaluations (Random Search mode), so an interrupted run — crash, manual kill, etc. — can always be resumed from the most recent checkpoint by passing the `.chk` file as the program argument instead of the `.ini` file.

If the run completes successfully (reaches the configured number of generations/iterations, exhausts its budget, or stops early via the patience mechanism), the checkpoint file is deleted automatically, since it's no longer needed.

**ATTENTION:** Checkpoint files **must not** be edited or modified by hand. The parser expects an exact, program-generated format and does not validate its contents.

## Fitness function

There is only one fitness function at the moment, using TOMTOM's $e$-value:

$$
f(x) = - \log_{10}(\min e\text{-value})
$$

## Outputs

Both algorithms generate CSVs following a naming convention and internal structure.

### Genetic Algorithm
`generations_<generations>_<population>.csv`

| Name | Description |
|------|-------------|
| Generation | Current generation index |
| Fitness | Best fitness found up to this generation |
| Params | MACS3 command string for the best individual |
| Budget | Unique evaluations performed in this generation |
| Time | Elapsed time in seconds |

### Random Search
`random_generations_<number of iterations>_<points per iteration>.csv`

| Name | Description |
|------|-------------|
| Iter | Current iteration index |
| Fitness | Best fitness found up to this iteration |
| Params | MACS3 command string for the best point |
| Time | Elapsed time in seconds |

	P.S: Random Search doesn't have a "budget" column because it always executes its full computed budget.

# Contributing
There's two ways that you can contribute with the project

## Code
- [ ] Parallel version
- [ ] Support for other types of files and fitness functions
	- [ ] Mean of $e$-value
	- [ ] Median of $e$-value
- [ ] Code optimization and refactoring
- [ ] Dynamic makefile generation based on computer configurations
- [ ] Documentation

If you change how arguments are parsed, checkpoint format, or the program name, **please** make sure you update the corresponding sections of this document!

## Bioinformatic
- [ ] Testing on other pipelines
- [ ] Testing in other genome data such as mm
- [ ] Comparison with other hyperparameter methods


# License
This project uses the MIT LICENSE, check [LICENSE](LICENSE) for more information

# Citation
One day...